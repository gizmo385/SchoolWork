package edu.arizona.cachapline8;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.stream.Collectors;
import java.util.*;

import edu.arizona.sista.processors.*;
import edu.arizona.sista.processors.fastnlp.FastNLPProcessor;

public class LanguageModel {
    // Determines considerations between the document model and collection model
    private static final double LAMBDA = 0.5;

    // Set up the processor
    private static Processor proc = new FastNLPProcessor(true, false, false);

    // { term : { documentName : frequency } }
    private static Map<String, Map<String, Integer>> termMaps = new HashMap<>();

    // { term : global term count }
    private static Map<String, Integer> globalTermCounts = new HashMap<>();

    // { documentName : length }
    private static Map<String, Integer> documentLengths = new HashMap<>();

    private static int numberOfTokens = 0;

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
                int currentFrequency = termMap.getOrDefault(documentName, 0);
                termMap.put(documentName, currentFrequency + 1);
                termMaps.put(term, termMap);

                int currentGlobalCount = globalTermCounts.getOrDefault(term, 0);
                globalTermCounts.put(term, currentGlobalCount + 1);

                documentLength += 1;
            }
        }

        numberOfTokens += documentLength;
        documentLengths.put(documentName, documentLength);
    }

    public static double handleQueryTermInDocument(String term, String documentName) {
        if(termMaps.containsKey(term)) {
            Map<String, Integer> termMap = termMaps.getOrDefault(term, new HashMap<String, Integer>());

            // Calculate the value for the document
            int documentFreq = termMap.getOrDefault(documentName, 0);
            double documentTermValue = (double) documentFreq / documentLengths.get(documentName);

            // Calculate the value for the collection
            int collectionFreq = globalTermCounts.getOrDefault(term, 0);
            double collectionTermValue = (double) collectionFreq / numberOfTokens;

            // Smooth the document and collection values
            return (LAMBDA * documentTermValue) + ((1 - LAMBDA) * collectionTermValue);
        } else {
            return LAMBDA * (1.0 / numberOfTokens);
        }
    }

    public static double handleQueryInDocument(String query, String documentName) {
        double ranking = 1;

        for(String term : query.split("\\s+")) {
            double queryTermValue = handleQueryTermInDocument(term, documentName);
            ranking *= queryTermValue;
        }

        return ranking;
    }

    public static Map<String, Double> rankDocuments(String query, Set<String> documents) {
        Map<String, Double> rankings = new HashMap<>();

        for(String documentName : documents) {
            rankings.put(documentName, handleQueryInDocument(query, documentName));
        }

        return rankings;
    }

    public static Set<String> getDocumentList(String query) {
        Set<String> documents = new TreeSet<>();

        for(String term : query.split("\\s+")) {
            documents.addAll(termMaps.getOrDefault(term, new HashMap<String, Integer>()).keySet());
        }

        return documents;
    }

    public static void executeQuery(String query) {
        Set<String> documents = getDocumentList(query);
        Map<String, Double> rankings = rankDocuments(query, documents);

        // Sort the documents based on the rankings
        List<String> byRank = new ArrayList<>(documents);
        Collections.sort(byRank, (d1, d2) -> Double.compare(rankings.get(d1), rankings.get(d2)));

        // Print the documents
        for(String documentName : byRank) {
            System.out.printf("%s (%f)\n", documentName, rankings.get(documentName));
        }
    }

    public static void main(String [] filenames) throws Exception {
        // Read in the documents
        for(String filename : filenames) {
            Files.lines(Paths.get(filename)).forEach(LanguageModel::parseDocument);
        }

        // Read user queries
        Scanner input = new Scanner(System.in);
        while(true) {
            System.out.print("Enter a query (exit to quit): ");
            String query = input.nextLine();

            if("exit".equals(query)) {
                break;
            } else {
                executeQuery(query);
            }
        }
    }
}
