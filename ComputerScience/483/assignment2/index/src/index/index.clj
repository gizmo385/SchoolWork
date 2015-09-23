(ns index.index
  "Functions to the create and search an inverted index structure."
  (:require [clojure.string :refer [split lower-case split-lines] :as s]
            [clojure.edn :as edn]
            [clojure.algo.generic.functor :refer [fmap]]))

;;; Defining and creating an inverted index
(defrecord InvertedIndex [documents index])
(defrecord Posting [document-id positions])

(defn- enumerate
  "Zips a collection with an zero-index range to create an enumerated list.

   Example: (enumerate ['a 'b 'c 'd]) --> ([0 'a] [1 'b] [2 'c] [3 'd])"
  [coll]
  (map vector (range) coll))

(defn- gather-positions
  "Tokenizes the string and maps each token in the string a singleton list containing only the
   doc id"
  [string]
  (loop [strings (enumerate (rest (split string #"\s+")))
         acc {}]
    (if-let [[position string] (first strings)]
      (recur (rest strings)
             (assoc acc string (cons position (get acc string ()))))
      (fmap sort acc))))

(defn- tokenize
  "Tokenizes the string based on the positions of the tokens in the document"
  [doc-id string]
  (for [[string positions] (gather-positions string)]
    {string (list (Posting. doc-id positions))}))

(defn- postings-list
  "Creates a postings list for the documents supplied."
  [documents]
  (flatten
    (for [[document-id document] (enumerate documents)]
      (tokenize document-id document))))

(defn- doc-id-map
  "Zips a range (0...n) where n is the number of documents with the list of document names."
  [documents]
  (zipmap (range (count documents))
          (for [doc documents] (first (split doc #"\s+")))))

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
  (->> filename
       slurp
       split-lines
       inverted-index))

;;; Searching our inverted index
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
      (let [term1-first (first term1-documents)
            term2-first (first term2-documents)
            doc-id-compare (compare (:document-id term1-first) (:document-id term2-first))]
        (cond
          (= 0 doc-id-compare) (recur (conj documents term1-first)
                                      (next term1-documents)
                                      (next term2-documents))
          (< 0 doc-id-compare) (recur documents term1-documents (next term2-documents))
          (> 0 doc-id-compare) (recur documents (next term1-documents) term2-documents)))
      documents)))

(defmethod search-index-op :prox [operator term1-documents term2-documents]
  (let [proximity (Integer/parseInt (subs (str operator) 1))]
    (println proximity)))

;;; Parsing search queries

(defn- handle-query
  "Handles query operators (AND, OR, etc.) as a reduction."
  [{:keys [index] :as inverted-index} query]
  (let [handle (partial handle-query inverted-index)]
    (cond
      ;; Base cases
      (operator? query) query
      (symbol? query) (get index (str query))
      (empty? query) ()

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
         (str "(" query ")")
         (edn/read-string query))
    (catch RuntimeException re nil)))

(defn search-index
  "Searches the index by combining the supplied terms with a particular operation. This returns a
   list of document ids"
  [{:keys [documents index] :as inverted-index} query]
  (if (empty? query)
    ()
    (if-let [parsed-query (read-query query)]
      (for [postings (handle-query inverted-index parsed-query)]
        (get documents (:document-id postings)))
      "Error: Unbalanced parenthesis in query")))
