(ns index.core
  (:require [index.index :refer [search-index inverted-index]]
            [clojure.string :refer [split split-lines trim join]]
            [clojure.pprint :refer [pprint]])
  (:gen-class))

(def ^:dynamic *prompt*
  "This is the prompt text that will be printed while asking for user queries."
  ">>> ")

(defn- prompt
  "Writes the prompt to stdout and then reads input from stdin. This prompt is specified in the by
   the *prompt* variable."
  []
  (print *prompt*)
  (flush)
  (read-line))

(defn- start-query-prompt
  "Starts a prompt that listens for queries and then makes those queries against the supplied
   index."
  [index]
  (loop [query-text (prompt)]
    (when (not-empty query-text)
      (println (search-index index query-text))
      (recur (prompt)))))

(defn -main
  "First we construct the inverted index. The first argument to the program should be a filename
   where each line in the file is a string of text, which will be treated as a document.  Arguments
   supplied after this will be assumed to be your search query terms. Arguments supplied to this
   program will be by assuming an implicit AND exists between elements"
  [& args]
  (if (not-empty args)
    (let [[document & query-terms] args
          index (->> document
                     slurp              ; We should read the file
                     split-lines        ; Capture the lines
                     (map trim)         ; Trim each line
                     inverted-index)]   ; And construct our inverted index based on the lines
      (pprint (:index index))
      (pprint (:documents index))
      (if (not-empty query-terms)
        (println (search-index index (join " " query-terms)))
        (start-query-prompt index)))
    (binding [*out* *err*]
      (println "ERROR: NO DOCUMENT SUPPLIED\n")
      (println "Proper usage: lein run <filename> [query-terms ...]")
      (println "Filename ensure that the first 2 words form the name of the document.")
      (println "Query format: <term> [[operator term] ...] where operator is one of: OR, AND")
      (System/exit 1))))
