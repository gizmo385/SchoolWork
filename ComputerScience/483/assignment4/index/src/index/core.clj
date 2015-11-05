(ns index.core
  (:require [clojure.string :refer [join split split-lines]]
            [clojure.java.io :refer [as-file]]
            [clojure.pprint :refer [pprint]]
            [index.lucene :refer [create-index search-index]])
  (:gen-class))

;; Some utility functions
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

(defn file-parser [line]
  (let [[document-name & contents] (split line #":")]
    (printf "Document name: %s\n" document-name)
    (printf "Contents: %s\n" (apply str contents))
    [{:type :string :name "document-name" :contents document-name :store true}
     {:type :text :name "contents" :contents (apply str contents) :store false}]))

;; Main program loop
(defn -main [& filenames]
  (let [index (create-index (flatten (map (comp split-lines slurp) filenames)) file-parser)]
    (try
      (loop [query (prompt)]
        (let [results (search-index index query "contents")
              num-results (count results)]
          (printf "Results found: %d\n" num-results))
        (recur (prompt)))
      (catch NullPointerException _ (do (println) (System/exit 0))))))
