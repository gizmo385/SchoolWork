(ns index.core
  (:require [index.index :refer [search-index inverted-index]]
            [clojure.string :refer [split split-lines trim]])
  (:gen-class))

(defn -main
  "First we construct the inverted index. The first argument to the program should be a filename
   where each line in the file is a string of text, which will be treated as a document. Arguments
   supplied after this will be assumed to be your search query terms."
  [& args]
  (let [index (->> args
                   first              ; The first element is our file containing documents
                   slurp              ; We should read the file
                   split-lines        ; Capture the lines
                   (map trim)         ; Trim each line
                   inverted-index)    ; And construct our inverted index based on the lines
        query (if (not-empty (rest args))
                (rest args)
                (->> *in*
                     java.io.BufferedReader.  ; We should open a reader on stdin
                     line-seq                 ; Get a sequence of lines from that reader
                     (map #(split %1 #"\s+")) ; Split the line based on spaces
                     flatten))]               ; And then flatten
    (println (search-index index :and query))))
