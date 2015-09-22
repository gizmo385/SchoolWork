(ns index.core
  (:require [index.index :refer [search-index file->index]]
            [index.repl :refer [search-index-loop]]
            [clojure.string :refer [join]]
            [clojure.pprint :refer [pprint]])
  (:gen-class))

(defn -main
  "First we construct the inverted index. The first argument to the program should be a filename
   where each line in the file is a string of text, which will be treated as a document.  Arguments
   supplied after this will be assumed to be your search query terms."
  [& args]
  (if (not-empty args)
    (let [[filename & query-terms] args
          index (file->index filename)]
      (if (not-empty query-terms)
        (println (search-index index (join " " query-terms)))
        (search-index-loop index)))
    (binding [*out* *err*]
      (println "ERROR: NO DOCUMENT SUPPLIED\n")
      (println "Proper usage: lein run <filename> [query-terms ...]")
      (println "Filename ensure that the first 2 words form the name of the document.")
      (println "Query format: <term> [[operator term] ...] where operator is one of: OR, AND")
      (System/exit 1))))
