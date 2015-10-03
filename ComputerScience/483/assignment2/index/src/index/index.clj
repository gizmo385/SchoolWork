(ns index.index
  "Functions to the create and search an inverted index structure."
  (:require [clojure.string :refer [split lower-case split-lines join] :as s]
            [clojure.edn :as edn]
            [clojure.algo.generic.functor :refer [fmap]]
            [clojure.java.io :refer [as-file]]))

;;; Defining and creating an inverted index
(defrecord InvertedIndex [documents index])
(defrecord TermOccurrence [document-id positions])

(defn- enumerate
  "Zips a collection with an zero-index range to create an enumerated list.

   Example: (enumerate ['a 'b 'c 'd]) --> ([0 'a] [1 'b] [2 'c] [3 'd])"
  [coll]
  (map vector (range) coll))

(defn- gather-positions
  "Tokenizes the string and maps each token in the string to a list containing the positions that
   the token occurs at within the string."
  [string]
  (loop [strings (enumerate (rest (split string #"\s+")))
         acc {}]
    (if-let [[position string] (first strings)]
      (recur (rest strings)
             (assoc acc string (cons position (get acc string ()))))
      (fmap sort acc))))

(defn- tokenize
  "Tokenizes a string from a particular document. This will create a map where the tokens in the
   string maps to a list of postings, where each posting contains the document id and the positions
   within the document that the string occurs at.

   Example: For the document 'Doc1 This is a test of this thing' in a document with id 0, then
   the following structure would be returned:
   ({this ({:document-id 0 :positions (0)})}
    {is ({:document-id 0 :positions (1)})}
    {a ({:document-id 0 :positions (2)})}
    {test ({:document-id 0 :positions (3)})}
    {of ({:document-id 0 :positions (4)})}
    {this ({:document-id 0 :positions (5)})}
    {thing ({:document-id 0 :positions (6)})})

   Note that Doc1 is ignored. The first token in the string is assumed to be the document name."
  [doc-id string]
  (for [[string positions] (gather-positions string)]
    {string (list (TermOccurrence. doc-id positions))}))

(defn- postings-list
  "Creates a postings list for the documents supplied. This returns a list of individual postings,
   which contain a single occurrence of a term in a single document. This can be combined (and are
   later) by mapping applying (merge-with concat) to the list."
  [documents]
  (flatten
    (for [[document-id document] (enumerate documents)]
      (tokenize document-id document))))

(defn- doc-id-map
  "Zips an infinite range with the list of document names"
  [documents]
  (zipmap (range) (for [doc documents] (first (split doc #"\s+")))))

(defn inverted-index
  "Constructs an inverted index for the text documents supplied. This first creates a postings list
   and then merges identical terms by concatenating their document lists together"
  [documents]
  ;; Each word should be a string containing the document
  (let [index (->> documents
                   (postings-list)
                   (apply merge-with concat))]
    (InvertedIndex. (doc-id-map documents) index)))

(defn file->index
  "Constructs an inverted index based on the lines in the file supplied"
  [filename]
  (if (.exists (as-file filename))
    (->> filename
         slurp
         split-lines
         inverted-index)
    (binding [*out* *err*]
      (printf "The file \"%s\" does not exist and cannot be indexed!\n" filename)
      (flush)
      (System/exit 1))))

;;; Searching our inverted index
(defrecord PositionalResult [posting positions])
(def ^:private operators ["AND" "OR"])

(defn- operator?
  "Returns whether or not a particular string is an operator"
  [text]
  (let [text (str text)]
    (or (= (first text) \_)
        (some #{text} operators))))

(defn- operator->keyword
  "Returns the correct keyword operator a particular operation."
  [operator]
  (cond
    (= (first (str operator)) \_) :prox
    :else (keyword (lower-case operator))))

(defmulti search-index-op
  "Implements different search operations"
  (fn [op term1-results term2-results] (operator->keyword op)))

(defmethod search-index-op :and [_ term1-documents term2-documents]
  (loop [documents []
         term1-documents term1-documents
         term2-documents term2-documents]
    ;; If one of the documents lists is empty, then there will be nothing in common
    (if (and term1-documents term2-documents)
      (let [term1-occurrence (first term1-documents)
            term2-occurrence (first term2-documents)
            doc-id-compare (compare (:document-id term1-occurrence)
                                    (:document-id term2-occurrence))]
        (cond
          (= 0 doc-id-compare) (recur (conj documents term1-occurrence)
                                      (next term1-documents)
                                      (next term2-documents))
          (< 0 doc-id-compare) (recur documents term1-documents (next term2-documents))
          (> 0 doc-id-compare) (recur documents (next term1-documents) term2-documents)))
      documents)))

(defn- check-proximity [proximity term1-positions term2-positions]
  (loop [term1-positions term1-positions
         term2-positions term2-positions]
    (if (and term1-positions term2-positions)
      (let [first-position (first term1-positions)
            second-position (first term2-positions)
            distance (Math/abs (- first-position second-position))
            position-comparison (compare first-position second-position)]
        (cond
          (<= distance proximity) (list first-position second-position)
          (< 0 position-comparison) (recur (next term1-positions) term2-positions)
          (> 0 position-comparison) (recur term1-positions (next term2-positions))))
      nil)))

(defmethod search-index-op :prox [operator term1-documents term2-documents]
  (loop [documents []
         term1-documents term1-documents
         term2-documents term2-documents]
    (if (and term1-documents term2-documents)
      (let [term1-occurrence (first term1-documents)
            term2-occurrence (first term2-documents)
            id-compare (compare (:document-id term1-occurrence) (:document-id term2-occurrence))]
        (cond
          (< 0 id-compare) (recur documents term1-documents (next term2-documents))
          (> 0 id-compare) (recur documents (next term1-documents) term2-documents)

          ;; Check proximities for identical terms
          (= 0 id-compare)
          (let [proximity (Integer/parseInt (subs (str operator) 1))]
            (if-let [positions (check-proximity proximity
                                                (:positions term1-occurrence)
                                                (:positions term2-occurrence))]
              (recur (conj documents (PositionalResult. term1-occurrence positions))
                     (next term1-documents)
                     (next term2-documents))
              (recur documents
                     (next term1-documents)
                     (next term2-documents))))))
      documents)))

(defn- transform-phrase-query
  "This function will take a phrase query like:
      'this is a phrase query'
   And restructure it like so:
      ((this /1 is) AND (is /1 a) AND (a /1 phrase) AND (phrase /1 query))"
  [terms]
  (as-> terms terms
    (partition 2 1 terms) ; "hey a phrase query" --> ((hey a) (a phrase) (phrase query))
    (map (fn [[t1 t2]] (list t1 '_1 t2)) terms) ; Add proximity operator to intermediate queries
    (interleave (repeat 'AND) terms) ; Separate subqueries with AND operator
    (rest terms) ; Drop the leading AND
    (edn/read-string (str terms)))) ; Read it into a form

(defn- phrase-query
  "Takes a phrase query and reparses it so that it is properly handled by the system."
  [query]
  (let [terms (split query #"\s+")
        term-count (count terms)]
    (cond
      (= 1 term-count) (symbol (first terms))
      (zero? term-count) ()
      :else (transform-phrase-query terms))))

;;; Parsing search queries

(defn- handle-query
  "Handles query operators (AND, OR, etc.) as a reduction. The reduction function works using the
   following method:

   For a query such as: term1 AND term2 /2 term3, it would be destructured as:
   [term1 [AND term2] [/2 term3]]

   The documents associated with term1 would be fetched and used as the initial value in the
   reduction. Then, the AND implementation would be called where the initial value and the second
   operand (term2 in this case) are passed as operands. The return of this would be used as the
   initial value the next time around the reduction. Nested queries are handled recursively."
  [{:keys [index] :as inverted-index} query]
  (let [handle (partial handle-query inverted-index)]
    (cond
      ;; Base cases
      (operator? query) query
      (symbol? query) (get index (str query))
      (empty? query) ()
      (string? query) (handle (phrase-query query))

      ;; Recursive case
      (list? query)
      (let [[first-term & remaining-terms] query]
        (reduce (fn [documents-acc [operation term]]
                  (search-index-op operation documents-acc (handle term)))
                (handle first-term)
                (partition 2 remaining-terms))))))

(defn- read-query
  "We read in a particular query by interpreting it as a parenthesized expression in EDN notation,
   which is described here: https://github.com/edn-format/edn"
  [raw-query]
  (try
    (as-> raw-query query
      (s/replace query "/" "_") ; This is for proximity queries. / cannot be in symbols, _ can
      (str "(" query ")") ; Ensure entire query is parsed by wrapping it in parens
      (edn/read-string query))
    (catch RuntimeException re nil)))

(defn- handle-result [result {:keys [documents]}]
  (cond
    (instance? PositionalResult result)
    (apply format "%s (%d, %d)"
           (get documents (-> result :posting :document-id))
           (:positions result))
    (instance? TermOccurrence result) (get documents (:document-id result))))

(defn search-index
  "Searches the index by combining the supplied terms with a particular operation. This returns a
   list of document ids"
  [{:keys [documents index] :as inverted-index} query]
  (if (empty? query)
    ()
    (if-let [parsed-query (read-query query)]
      (try
        (for [result (handle-query inverted-index parsed-query)]
          (handle-result result inverted-index))
        (catch IllegalArgumentException iae "There was an error while processing your query."))
      "Error: Unbalanced elements (parenthesis, quotations, etc) in query")))
