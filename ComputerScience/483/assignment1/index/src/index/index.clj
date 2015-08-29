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
    (InvertedIndex. documents (sort-by key (zipmap (keys index)
                                                   (map sort (vals index)))))))

(clojure.pprint/pprint (postings-list test-documents))

(clojure.pprint/pprint (inverted-index test-documents))
