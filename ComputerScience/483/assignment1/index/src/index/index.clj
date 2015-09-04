(ns index.index
  (:require [clojure.string :refer [join trim split]]))

(defn tokenize
  "Tokenizes the string and maps each token in the string a singleton list containing only the
   doc id"
  [doc-id string]
  (let [strings (split string #"\s+")]
    (for [string strings]
      {string (list doc-id)})))

(defn postings-list
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

(defn inverted-index
  "Constructs an inverted index for the text documents supplied. This first creates a postings list
   and then merges identical terms by concatenating their document lists together"
  [documents]
  ;; Each word should be a string containing the document
  (let [index (->> documents
                   (postings-list)
                   (apply (partial merge-with concat)))]
    (InvertedIndex. documents (zipmap (keys index)
                                      (map sort (vals index))))))

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

(defn search-index
  "Searches the index by combining the supplied terms with a particular operation. This returns a
   list of document ids"
  [{:keys [documents index]} op terms]
  (let [num-terms (count terms)]
    (cond
      (zero? num-terms) '()
      (= 1 num-terms) (get index (first terms) '())
      :else (->> terms
                 (map (partial get index))
                 (sort-by count)
                 (reduce (partial search-index-op op))))))
