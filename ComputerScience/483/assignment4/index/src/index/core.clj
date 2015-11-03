(ns index.core
  (:require [clojure.string :refer [join split split-lines]])
  (:import [org.apache.lucene.analysis.standard StandardAnalyzer]
           [org.apache.lucene.util Version]
           [org.apache.lucene.store RAMDirectory Directory]
           [org.apache.lucene.index IndexReader IndexWriter IndexWriterConfig DirectoryReader]
           [org.apache.lucene.document Field$Store StringField TextField Document]
           [org.apache.lucene.queryparser.classic QueryParser]
           [org.apache.lucene.search IndexSearcher Query TopScoreDocCollector ScoreDoc Sort]
           [java.io FileReader StringReader])
  (:gen-class))

;; Some global settings
(def lucene_version Version/LUCENE_5_3_1)
(def text-analyzer (StandardAnalyzer.))
(def ^:dynamic *hits-per-page* 10)
(def ^:dynamic *prompt*
  "This is the prompt text that will be printed while asking for user queries."
  "Enter your query: ")

(defn- prompt
  "Writes the prompt to stdout and then reads input from stdin. This prompt is specified in the by
   the *prompt* variable."
  []
  (print *prompt*)
  (flush)
  (read-line))

;; Indexing and searching
(defn create-document
  "Creates a document object based on the contents if a file."
  [document-name contents]
  (doto (Document.)
    (.add (StringField. "document-name" document-name Field$Store/YES))
    (.add (TextField. "contents" (StringReader. contents)))))

(defn add-to-index [writer document-name contents]
  (.addDocument writer (create-document document-name contents)))

(defn search-index
  "Searches the index and returns the documents that match the search."
  [index query & {:keys [field] :or {field "contents"}}]
  (let [query  (.parse (QueryParser. field text-analyzer) query)
        reader        (DirectoryReader/open index)
        searcher      (IndexSearcher. reader)
        collector     (TopScoreDocCollector/create *hits-per-page*)]
    (doto searcher
      (.search query collector))
    (for [result (.scoreDocs (.topDocs collector))]
      [(.score result) (.doc searcher (.doc result))])))

;; Main program loop
(defn -main [& filenames]
  (let [index (RAMDirectory.)]
    (with-open [writer (IndexWriter. index (IndexWriterConfig. text-analyzer))]
      (doto writer
        (add-to-index "Document #1" "information retrieval is the most awesome class I ever took.")
        (add-to-index "Document #2" "the retrieval of private information from your emails is a job that the NSA loves.")
        (add-to-index "Document #3" "in the school of information you can learn about data science.")
        (add-to-index "Document #4" "the labrador retriever is a great dog.")))
    (try
      (loop [query (prompt)]
        (let [results (search-index index query)
              num-results (count results)]
          (printf "Results found: %d\n" num-results)
          (println (join ", " (for [[score document] results]
                                (format "%s(%f)" (.get document "document-name") score)))))
        (recur (prompt)))
      (catch NullPointerException _ (do (println) (System/exit 0))))))
