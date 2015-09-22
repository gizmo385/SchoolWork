(ns index.util
  "Functions related to the search repl"
  (:require [index.index :refer [search-index inverted-index]]
            [clojure.string :refer [split split-lines trim join]]))

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

(defn search-index-loop
  "Starts a prompt that listens for queries and then makes those queries against the supplied
   index."
  [index]
  (loop [query-text (prompt)]
    (when (not-empty query-text)
      (println (search-index index query-text))
      (recur (prompt)))))
