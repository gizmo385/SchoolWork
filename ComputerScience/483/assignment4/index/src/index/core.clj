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

(defn file-parser [filename]
  (let [lines (split-lines (slurp filename))]
    (for [line lines :let [[document-name contents] (split line #":" 2)]]
      [{:type :string :name "document-name" :contents document-name :store true}
       {:type :text :name "contents" :contents contents :store false}])))

;; Main program loop
(defn -main [& filenames]
  (let [index (create-index filenames file-parser)]
    (try
      (loop [query (prompt)]
        (let [[searcher results] (search-index index "contents" query)]
          (printf "Found %d results:\n" (count results))
          (doseq [result results :let [document (.doc searcher (.doc result))]]
            (printf "%s(%f)\n" (.get document "document-name") (.score result))))
        (recur (prompt)))
      (catch NullPointerException _ (do (println) (System/exit 0))))))
