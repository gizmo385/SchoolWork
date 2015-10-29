(ns index.lucene.core
  "Serves as a clojure wrapper around Apache Lucene."
  (:require [clojure.java.io :refer [as-file]])
  (:import [org.apache.lucene.analysis.standard StandardAnalyzer]
           [org.apache.lucene.util Version]
           [org.apache.lucene.store RAMDirectory Directory]
           [org.apache.lucene.index IndexReader IndexWriter IndexWriterConfig DirectoryReader]
           [org.apache.lucene.document Field$Store StringField TextField Document]
           [org.apache.lucene.queryparser.classic QueryParser]
           [org.apache.lucene.search IndexSearcher Query TopScoreDocCollector ScoreDoc]
           [java.io FileReader]))

;; Some global settings
(def lucene_version Version/LUCENE_5_3_1)
(def text-analyzer (StandardAnalyzer.))
(def ^:dynamic *hits-per-page* 10)

;; Index creation
(defn document-from-file
  "Creates a document object based on the contents if a file."
  [filename]
  (let [f (as-file filename)]
    (doto (Document.)
      (.add (StringField. "filename" (.getName f) Field$Store/YES))
      (.add (StringField. "path" (.getPath f) Field$Store/YES))
      (.add (TextField. "contents" (FileReader. f))))))

(defn add-files-to-index
  "Indexes a series of files and adds them to the supplied index. Returns the index."
  [index filenames]
  (let [documents (map document-from-file filenames)]
    (with-open [writer (IndexWriter. index (IndexWriterConfig. text-analyzer))]
      (doseq [document documents]
        (.addDocument writer document)))
    index))

(defn create-index
  "Creates an index and adds the supplied files to it."
  [& filenames]
  (doto (RAMDirectory.)
    (add-files-to-index filenames)))

;; Searching the index
(defn search-index
  "Searches the index and returns the documents that match the search."
  [index query & {:keys [field] :or {field "contents"}}]
  (let [query  (.parse (QueryParser. field text-analyzer) query)
        reader        (DirectoryReader/open index)
        searcher      (IndexSearcher. reader)
        collector     (TopScoreDocCollector/create *hits-per-page*)]
    (.. searcher (search query collector))
    (for [result (.scoreDocs (.topDocs collector))]
      (.doc searcher (.doc result)))))
