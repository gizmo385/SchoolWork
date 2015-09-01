(ns index.core
  (:require [index.index :refer [search-index inverted-index]])
  (:gen-class))

(defn -main
  [& args]
  (let [index (inverted-index args)]
    (println (search-index :and index ["drug" "schizophrenia" "approach"]))
    )
  )
