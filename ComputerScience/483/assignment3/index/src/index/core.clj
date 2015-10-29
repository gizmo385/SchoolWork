(ns index.core
  (:require [clojure.string :refer [join]]
            (index.lucene [core :refer [create-index search-index]]))
  (:gen-class))

(def ^:dynamic *prompt*
  "This is the prompt text that will be printed while asking for user queries."
  "Enter your query: ")

(defn- prompt
  "Writes the prompt to stdout and then reads input from stdin. This prompt is specified in the by
   the *prompt* variable."
  []
  (print *prompt*)
  (flush)
  (read-line))

(defn -main [& filenames]
  (let [index (apply create-index filenames)]
    (loop [query (prompt)]
      (let [results (search-index index query)]
        (println (join ", "(map #(.get % "filename") results))))
      (recur (prompt)))))
