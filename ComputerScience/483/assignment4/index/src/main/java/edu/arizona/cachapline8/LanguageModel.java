package edu.arizona.cachapline8;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.stream.Collectors;
import java.util.*;

import edu.arizona.sista.processors.*;
import edu.arizona.sista.processors.fastnlp.FastNLPProcessor;

public class LanguageModel {
    // Set up the processor
    private static Processor proc = new FastNLPProcessor(true, false, false);

    // { term : { documentName : frequency } }
    private static Map<String, Map<String, Integer>> termMaps = new HashMap<>();

    // { documentName : length }
    private static Map<String, Integer> documentLengths = new HashMap<>();

    public static double maximumLiklihoodEstimate(String term, String documentName) {
        Map<String, Integer> termFrequencies = termMaps.getOrDefault(term, new HashMap<>());
        int termFrequency = termFrequencies.getOrDefault(documentName, 0);

        return termFrequency / documentLengths.getOrDefault(documentName, 1);
    }

    public static void parseDocument(String documentText) {
        int colon = documentText.indexOf(':');
        String documentName = documentText.substring(0, colon);

        Document document = proc.mkDocument(documentText.substring(colon + 1), false);
        document = proc.annotate(document);

        int documentLength = 0;
        for(Sentence sentence : document.sentences()) {
            for(String term : sentence.lemmas().get()) {
                // Update the term maps for each term
                Map<String, Integer> termMap = termMaps.getOrDefault(term, new HashMap<String, Integer>());
                termMap.put(documentName, termMap.getOrDefault(documentName, 0) + 1);
                termMaps.put(term, termMap);

                documentLength += 1;
            }
        }

        documentLengths.put(documentName, documentLength);
    }

    public static void main(String [] filenames) throws Exception {
        // Read in the documents
        for(String filename : filenames) {
            Files.lines(Paths.get(filename)).forEach(LanguageModel::parseDocument);
        }

        System.out.println("Terms:");
        termMaps.keySet().stream().forEach(System.out::println);
    }
}
