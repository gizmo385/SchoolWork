(ns index.index
  (require [clojure.string :refer [join trim split]]))

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

(clojure.pprint/pprint (inverted-index test-documents))

(defmulti search-index-op
  "Implements different search operations"
  (fn [op index term1-results term2-results] op))

(defmethod search-index-op :and [_ index term1-results term2-results]
  (loop [results []
         term1-results term1-results
         term2-results term2-results]
    (if (and (not-empty term1-results) (not-empty term2-results))
      (let [term1-first (first term1-results)
            term2-first (first term2-results)
            term1-shorter (< (count term1-results) (count term2-results))]
        (if (= term1-first term2-first)
          (recur (conj results term1-first)
                 (rest term1-results)
                 (rest term2-results))
          (recur results
                 (if term1-shorter term1-results (rest term1-results))
                 (if term1-shorter (rest term2-results) term2-results))))
      results)))

(defn search-index [op {:keys [documents index]} terms]
  (if (< 2 (count terms))
    (->> terms
         (map (partial get index))
         (sort-by count)
         (map (partial get documents)))
    (->> terms
         (map (partial get index))
         (sort-by count)
         (reduce (partial search-index-op op index))
         (map (partial get documents)))))

(comment
  (let [index (inverted-index test-documents)]
    (println (map slurp test-documents))
    (println (search-index :and index ["drug" "schizophrenia"]))
    (println (search-index :and index ["schizophrenia"]))
    (println (search-index :and index ["approach"]))
    ))
