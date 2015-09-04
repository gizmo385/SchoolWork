(ns index.core
  (:require [index.index :refer [search-index inverted-index]]
            [clojure.string :refer [split split-lines trim]])
  (:gen-class))

(defn parse-query [query]
  (map trim (split query #"\s+")))

(defn prompt [prompt-text]
  (flush)
  (print prompt-text)
  (flush)
  (read-line))

(defn- start-query-prompt [index]
  (loop [query-text (prompt ">>> ")]
    (when (not-empty query-text)
      (->> query-text
           parse-query
           (search-index index :and)
           println)
      (recur (prompt ">>> ")))))

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
      (start-query-prompt index))))
