(ns index.core
  (:require [index.index :refer [search-index inverted-index]]
            [clojure.string :refer [split split-lines trim]])
  (:gen-class))

(defn parse-query
  "Parses the query so that it can be sent to the index"
  [query]
  (map trim (split query #"\s+")))

(defn prompt
  "Writes the desired prompt to stdout and then reads input from stdin"
  [prompt-text]
  (flush)
  (print prompt-text)
  (flush)
  (read-line))

(defn- start-query-prompt
  "Starts a prompt that listens for queries and then makes those queries against the supplied
   index."
  [index prompt-text]
  (loop [query-text (prompt prompt-text)]
    (when (not-empty query-text)
      (->> query-text
           parse-query
           (search-index index :and)
           println)
      (recur (prompt prompt-text)))))

(defn -main
  "First we construct the inverted index. The first argument to the program should be a filename
   where each line in the file is a string of text, which will be treated as a document. Arguments supplied after this will be assumed to be your search query terms."
  [& args]
  (let [index (->> args
                   first              ; The first element is our file containing documents
                   slurp              ; We should read the file
                   split-lines        ; Capture the lines
                   (map trim)         ; Trim each line
                   inverted-index)]   ; And construct our inverted index based on the lines
    (if (not-empty (rest args))
      (println (search-index index :and (rest args)))
      (start-query-prompt index ">>> "))))
