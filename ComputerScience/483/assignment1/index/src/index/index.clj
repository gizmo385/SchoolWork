(ns index.index
  (:require [clojure.string :refer [join trim split]]))

(def test-documents ["doc1" "doc2" "doc3" "doc4"])

(defn tokenize
  "Tokenizes the string and maps each token in the string a singleton list containing only the
   doc id"
  [doc-id string]
  (let [strings (split string #"\s+")]
    (for [string strings]
      {string (list doc-id)})))

(defn postings-list [documents]
  (loop [documents documents
         doc-id-counter 0
         acc []]
    (if (not-empty documents)
      ;; Read, trim, and tokenize each document
      (let [document-postings (->> documents
                                   (first)
                                   (slurp)
                                   (trim)
                                   (tokenize doc-id-counter))]
        (recur (rest documents)
               (inc doc-id-counter)
               (cons document-postings acc)))
      (flatten acc))))

(defrecord InvertedIndex [documents index])

(defn inverted-index
  "Constructs an inverted index for the documents supplied."
  [documents]
  ;; Each word should map to a list of documents containing it
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
    (if (and (not-empty term1-documents) (not-empty term2-documents))
      (let [term1-first (first term1-documents)
            term2-first (first term2-documents)
            term1-lower (< term1-first term2-first)]
        (if (= term1-first term2-first)
          (recur (cons term1-first documents)
                 (rest term1-documents)
                 (rest term2-documents))
          (recur documents
                 (if term1-lower (rest term1-documents) term1-documents)
                 (if term1-lower term2-documents (rest term2-documents)))))
        documents)))

(defn search-index [op {:keys [documents index]} terms]
  (if (< (count terms) 2)
    (->> terms
         (map (partial get index))
         (sort-by count)
         (map (partial nth documents)))
    (->> terms
         (map (partial get index))
         (sort-by count)
         (reduce (partial search-index-op op))
         (map (partial nth documents)))))
