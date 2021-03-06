(ns index.index
  (:require [clojure.string :refer [join trim split lower-case] :as s]
            [clojure.edn :as edn]))

;;; Defining and creating an inverted index

(defn- tokenize
  "Tokenizes the string and maps each token in the string a singleton list containing only the
   doc id"
  [doc-id string]
  (let [strings (rest (split string #"\s+"))]
    (for [string strings]
      {string (list doc-id)})))

(defn- postings-list
  "Creates a postings list for the documents supplied."
  [documents]
  (loop [documents documents
         doc-id-counter 0
         acc []]
    (if (not-empty documents)
      ;; Read, trim, and tokenize each document
      (let [document-postings (->> documents
                                   (first)
                                   (trim)
                                   (tokenize doc-id-counter))]
        (recur (rest documents)
               (inc doc-id-counter)
               (cons document-postings acc)))
      (flatten acc))))

(defrecord InvertedIndex [documents index])

(defn- doc-id-map [documents]
  (let [ids (range (count documents))
        doc-names (for [doc documents] (first (split doc #"\s+")))]
    (zipmap ids doc-names)))

(defn inverted-index
  "Constructs an inverted index for the text documents supplied. This first creates a postings list
   and then merges identical terms by concatenating their document lists together"
  [documents]
  ;; Each word should be a string containing the document
  (let [index (->> documents
                   (postings-list)
                   (apply (partial merge-with concat)))]
    (InvertedIndex. (doc-id-map documents)
                    (zipmap (keys index) (map sort (vals index))))))

;;; Searching our inverted index

(defmulti search-index-op
  "Implements different search operations"
  (fn [op term1-results term2-results] op))

(defmethod search-index-op :and [_ term1-documents term2-documents]
  (loop [documents []
         term1-documents term1-documents
         term2-documents term2-documents]
    ;; If one of the documents lists is empty, then there will be nothing in common
    (if (and (not-empty term1-documents) (not-empty term2-documents))
      (let [term1-first (first term1-documents)
            term2-first (first term2-documents)
            term1-lower (< term1-first term2-first)]
        (if (= term1-first term2-first)
          ;; Either we need to advance both document lists
          (recur (cons term1-first documents)
                 (rest term1-documents)
                 (rest term2-documents))
          ;; Or we need to a single document list
          (recur documents
                 ;; And the list we advance is the one with the smaller current number
                 (if term1-lower (rest term1-documents) term1-documents)
                 (if term1-lower term2-documents (rest term2-documents)))))
      documents)))

(defmethod search-index-op :or [_ term1-documents term2-documents]
  (distinct (concat term1-documents term2-documents)))

(defmethod search-index-op :prox [_ term1-documents term2-documents]
  nil)

(def ^:private operators ["AND" "OR"])

(defn- operator? [text]
  (let [text (str text)]
    (or (= (first text) \_)
        (some #{text} operators))))

(defn- handle-query
  "Handles query operators (AND, OR, etc.) as a reduction."
  [{:keys [index] :as inverted-index} query]
  (let [handle (partial handle-query inverted-index)]
    (cond
      ; Base cases
      (operator? query) query
      (symbol? query) (get index (str query))
      (empty? query) ()

      ; Recursive case
      (list? query)
      (let [[first-term & remaining-terms] query]
        (reduce (fn [documents-acc [operation term]]
                  (search-index-op (keyword (lower-case operation)) documents-acc (handle term)))
                  (handle first-term)
                  (partition 2 remaining-terms))))))

(defn- read-query
  "We read in a particular query by interpreting it as a parenthesized expression in EDN notation,
   which is described here: https://github.com/edn-format/edn"
  [query]
  (try
    (edn/read-string (str "(" (s/replace query "/" "_") ")"))
    (catch RuntimeException re nil)))

(defn search-index
  "Searches the index by combining the supplied terms with a particular operation. This returns a
   list of document ids"
  [{:keys [documents index] :as inverted-index} query]
  (if (empty? query)
    ()
    (if-let [parsed-query (read-query query)]
      (map (partial get documents)
           (handle-query inverted-index parsed-query))
      "Error: Unbalanced parenthesis in query")))
