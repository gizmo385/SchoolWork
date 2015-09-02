(ns index.core
  (:require [index.index :refer [search-index inverted-index]]
            [clojure.string :refer [split-lines trim]])
  (:gen-class))

(defn -main
  "First we construct the inverted index. The first argument to the program should be a filename
   where each line in the file is a string of text, which will be treated as a document. Arguments
   supplied after this will be assumed to be your search query terms."
  [& args]
  (let [index (->> args
                   (first)
                   (slurp)
                   (split-lines)
                   (map trim)
                   (inverted-index))
        query (if (not-empty (rest args))
                (rest args)
                (line-seq (java.io.BufferedReader. *in*)))]
    (println (search-index index :and query))))
