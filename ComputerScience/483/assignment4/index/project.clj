(defproject index "0.1.0-SNAPSHOT"
  :description "FIXME: write description"
  :url "http://example.com/FIXME"
  :license {:name "Eclipse Public License"
            :url "http://www.eclipse.org/legal/epl-v10.html"}
  :dependencies [[org.clojure/clojure "1.7.0"]
                 [org.apache.lucene/lucene-core "5.3.1"]
                 [org.apache.lucene/lucene-analyzers-common "5.3.1"]
                 [org.apache.lucene/lucene-queryparser "5.3.1"]]
  :main ^:skip-aot index.core
  :source-paths ["src"]
  :target-path "target/%s"
  :profiles {:uberjar {:aot :all :target-path "."}
             :repl {:dependencies [[org.clojure/tools.namespace "0.2.11"]]
                    :source-paths ["dev" "src"]
                    :main user}}
  :uberjar-name "index.jar")
