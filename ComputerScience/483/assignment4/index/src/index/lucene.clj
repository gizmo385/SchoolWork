(ns index.lucene
  (:require [clojure.string :refer [join split split-lines]]
            [clojure.java.io :refer [as-file file]])
  (:import [clojure.lang Reflector]
           [java.nio.file Paths Files]
           [java.net URI]
           [org.apache.lucene.analysis.standard StandardAnalyzer]
           [org.apache.lucene.util Version]
           [org.apache.lucene.store RAMDirectory FSDirectory Directory]
           [org.apache.lucene.index IndexReader IndexWriter IndexWriterConfig DirectoryReader]
           [org.apache.lucene.queryparser.classic QueryParser]
           [org.apache.lucene.search IndexSearcher Query TopScoreDocCollector ScoreDoc Sort]
           [org.apache.lucene.document Field$Store Document StringField TextField IntField
            LongField FloatField DoubleField])
  (:gen-class))

;; Some global settings
(def lucene_version Version/LUCENE_5_3_1)
(def text-analyzer (StandardAnalyzer.))
(def ^:dynamic *hits-per-page* 10)

;; Creating documents
(def field-classes
  {:string  StringField
   :text    TextField
   :int     IntField
   :long    LongField
   :float   FloatField
   :double  DoubleField})

(defn map->field
  "Constructs a document based upon the type supplied."
  [{:keys [type name contents store?]}]
  (Reflector/invokeConstructor
    (get field-classes type)
    (into-array Object [name contents (if store? Field$Store/YES Field$Store/NO)])))

(defn maps->document
  "Takes a series of maps representing fields in a document and creates a document from them."
  [maps]
  (let [document (Document.)]
    (doseq [field (map map->field maps)]
      (.add document field))
    document))

(defn build-documents
  "Builds a list of documents based on a list of data sources and some function that can parse
   those data sources. The data-parser function should take a data source and return a list of maps
   which can be supplied to maps->document to build a document.

   This function returns a list of documents."
  [data-sources data-parser]
  (map (comp maps->document data-parser) data-sources))

;; Index construction
(defn memory-index []
  (RAMDirectory.))

(defn disk-index [filename]
  (let [path (.toPath (file filename))]
    (FSDirectory/open path)))

(defn create-index
  "Creates an index and adds the supplied data sources to the index. An optional filename can be
   supplied, in which case a disk-based directory will be used. If such a filename is not supplied,
   then a RAM based directory will be used."
  [data-sources data-parser & {:keys [filename]}]
  (let [index (if filename (disk-index filename) (memory-index))]
    (with-open [writer (IndexWriter. index (IndexWriterConfig. text-analyzer))]
      (doseq [document (build-documents data-sources data-parser)]
        (.addDocument writer document)))
    index))

;; Searching through the index
(defn search-index
  "Searches the index and returns the documents that match the search."
  [index field query]
  (let [query         (.parse (QueryParser. field text-analyzer) query)
        reader        (DirectoryReader/open index)
        searcher      (IndexSearcher. reader)
        collector     (TopScoreDocCollector/create *hits-per-page*)]
    (doto searcher
      (.search query collector))
    (.scoreDocs (.topDocs collector))))
