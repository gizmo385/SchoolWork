(ns index.lucene
  (:require [clojure.string :refer [join split split-lines]]
            [clojure.java.io :refer [as-file]])
  (:import [clojure.lang Reflector]
           [java.io FileReader StringReader]
           [org.apache.lucene.analysis.standard StandardAnalyzer]
           [org.apache.lucene.util Version]
           [org.apache.lucene.store RAMDirectory Directory]
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

(comment
  ;; An example of building documents from a series of filenames
  (build-documents
    ["test2.txt"]
    (fn [filename]
      (let [f (as-file filename)]
        [{:type :string :name "filename" :contents (.getName f) :store true}
         {:type :string :name "path" :contents (.getPath f) :store true}
         {:type :text :name "contents" :contents (slurp f) :store false}])))
  )
