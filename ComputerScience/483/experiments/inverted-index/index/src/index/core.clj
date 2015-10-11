(ns index.core
  "This is the main entry point into the program. It provides only the main function, which creates
   the inverted index based on a supplied document source."
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
    (do
      (print "Enter a filename/path: ")
      (flush)
      (let [index (file->index (read-line))]
        (search-index-loop index)))))
