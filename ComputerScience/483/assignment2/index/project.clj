(defproject index "1.0"
  :description "An implementation of an inverted index in Clojure for Assignment 1 of CSc 483."
  :license {:name "Eclipse Public License"
            :url "http://www.eclipse.org/legal/epl-v10.html"}
  :dependencies [[org.clojure/clojure "1.6.0"]
                 [org.clojure/algo.generic "0.1.1"]]
  :main ^:skip-aot index.core
  :target-path "target/%s"
  :profiles {:uberjar {:aot :all}})
