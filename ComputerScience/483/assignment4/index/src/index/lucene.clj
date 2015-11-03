(ns index.lucene
  (:require [clojure.string :refer [join split split-lines]])
  (:import [clojure.lang Reflector]
           [java.io FileReader StringReader]
           [org.apache.lucene.analysis.standard StandardAnalyzer]
           [org.apache.lucene.util Version]
           [org.apache.lucene.store RAMDirectory Directory]
           [org.apache.lucene.index IndexReader IndexWriter IndexWriterConfig DirectoryReader]
           [org.apache.lucene.document Field$Store Document StringField TextField IntField
            LongField FloatField DoubleField]
           [org.apache.lucene.queryparser.classic QueryParser]
           [org.apache.lucene.search IndexSearcher Query TopScoreDocCollector ScoreDoc Sort])
  (:gen-class))

;; Some global settings
(def lucene_version Version/LUCENE_5_3_1)
(def text-analyzer (StandardAnalyzer.))
(def ^:dynamic *hits-per-page* 10)

;; Creating fields for the index
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
    (field-classes type)
    (into-array Object [name contents (if store? Field$Store/YES Field$Store/NO)])))

(defn maps->document
  "Takes a series of maps representing fields in a document and creates a document from them."
  [& maps]
  (let [document (Document.)]
    (doseq [field (map map->field maps)]
      (.add document field))
    document))

(comment
  (maps->document
    {:type :string :name "Test1" :contents "Testing" :store? true}
    {:type :string :name "Test2" :contents "Testing" :store? true}
    {:type :string :name "Test3" :contents "Testing" :store? true}
    {:type :text :name "Test4" :contents "Testing" :store? true}
    {:type :string :name "Test5" :contents "Testing" :store? true}
    {:type :string :name "Test6" :contents "Testing" :store? true}
    {:type :string :name "Test7" :contents "Testing" :store? true}))
