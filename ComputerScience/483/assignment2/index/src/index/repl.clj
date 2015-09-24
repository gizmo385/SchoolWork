(ns index.repl
  "Functions related to the search repl"
  (:require [index.index :refer [search-index inverted-index]]
            [clojure.string :refer [join split]]))

;;; The primary elements of the query loop
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

(defmulti handle-command (fn [command index] (first (split command #"\s+"))))

(defn search-index-loop
  "Starts a prompt that listens for queries and then makes those queries against the supplied
   index."
  [index]
  ;; Start of by printing help information
  (handle-command "!help" index)
  ;; Then begin the query loop
  (loop [query-text (prompt)]
    (when (not-empty query-text)
      (if (= (first query-text) \!) ; Check if the query is actually a REPL command
        (handle-command query-text index)
        (as-> (search-index index query-text) result
          (cond
            (string? result) result
            (not-empty result) (join ", " result)
            :else "No results found")
          (println result)))
      (recur (prompt)))))

;;; REPL command implementations
(defmethod handle-command :default [command _]
  (let [valid-commands (join ", " (filter string? (keys (methods handle-command))))]
    (printf "That is not a valid command! Valid commands are: %s\n" valid-commands)
    (println "Enter !help for more information.")))

(defmethod handle-command "!quit" [_ _]
  (println "Goodbye!")
  (System/exit 0))

(defmethod handle-command "!help" [_ _]
  (println
    """
    To use this inverted index, supply queries to the REPL in the form: [term [operator term]...].

    Example: (term1 AND (term2 /4 term3)) AND term5

    The currently supported operators are:
        Binary AND (term1 AND term2): Returns documents where both terms are present
        Proximity: (term1 /n term2): Returns documents where term2 is within n tokens of term1.

    Parenthesized queries are also supported. Additionally, there are the following commands:
       !help: Prints this message.
       !quit: Quits the query REPL
       !docs: Prints the names of the documents in the collection.
       !prompt [prompt]: Change the prompt to some specified string.
       !count [query]: Counts the results of the provided query.
    """))

(defmethod handle-command "!docs" [command index]
  (println (join ", " (sort (vals (:documents index))))))

(defmethod handle-command "!prompt" [command _]
  (let [[command & prompt] (split command #"\s+")]
    (if (not-empty prompt)
      (let [prompt (str (join " " prompt) " ")]
        (printf """Changing prompt from """)
        (printf "Changing prompt from \"%s\" to \"%s\"\n" *prompt* prompt)
        (alter-var-root #'*prompt* (constantly prompt)))
      (println "Error: A new prompt is required!"))))

(defmethod handle-command "!count" [command index]
  (let [[command & query] (split command #"\s+")]
    (printf "Your query returned %d results\n."
            (count (search-index index (join " " query))))))


