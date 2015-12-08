package edu.arizona.cachapline8;

import java.nio.file.*;
import java.io.*;
import java.util.stream.*;
import java.util.function.*;
import java.util.regex.*;
import java.util.*;

import org.apache.lucene.analysis.standard.*;
import org.apache.lucene.analysis.*;
import org.apache.lucene.analysis.en.*;
import org.apache.lucene.util.*;
import org.apache.lucene.store.*;
import org.apache.lucene.index.*;
import org.apache.lucene.document.*;
import org.apache.lucene.queryparser.classic.*;
import org.apache.lucene.search.*;
import org.apache.lucene.search.similarities.*;

public class Watson {

    private final static Pattern articleTitle = Pattern.compile("^\\[\\[(?<title>.+)\\]\\]$");
    private final static int HITS_PER_PAGE = 1;
    private final static String INDEX_FILENAME = "./watson_directory.dat";

    private static List<String> lines = new ArrayList<>();
    private static String currentArticleTitle = null;
    private static int numberOfArticles = 0;

    public static Document createDocument() {
        String contents = lines.stream().collect(Collectors.joining("\n"));

        Document d = new Document();
        d.add(new TextField("article_title", currentArticleTitle, Field.Store.YES));
        d.add(new TextField("contents", contents, Field.Store.NO));

        return d;
    }

    public static void handleLine(IndexWriter writer, String line) {
        Matcher m = articleTitle.matcher(line);
        if(m.matches()) {
            // A document out of the previous article
            if(currentArticleTitle != null) {
                Document d = createDocument();
                //System.out.printf("%d: %s\n", numberOfArticles, currentArticleTitle);
                numberOfArticles++;

                try {
                    writer.addDocument(d);
                } catch(IOException ioe) {
                    System.err.printf("Error while writing document!\n");
                }
            }

            // Prepare for the next article
            lines.clear();
            currentArticleTitle = m.group("title");

        } else {
            // If it isn't a title, add it to the contents for the filename
            lines.add(line.trim());
        }
    }

    public static void parseWikiDocument(Consumer<String> lineHandler, Path file) {
        String filename = file.toString();
        String currentArticleTitle = null;

        try {
            Files.lines(file).forEach(lineHandler);
        } catch(IOException ioe) {
            System.err.printf("Error while attempting to parse file(%s): %s\n", filename, ioe);
        }
    }

    public static void main(String[] directories) {
        // Set up the directory
        //Analyzer analyzer = new StandardAnalyzer();
        Analyzer analyzer = new EnglishAnalyzer();
        IndexWriterConfig config = new IndexWriterConfig(analyzer);

        try {
            File indexFile = new File(INDEX_FILENAME);
            Directory index;

            if(indexFile.exists()) {
                System.out.println("Directory already exists. Loading...");
                index = new SimpleFSDirectory(Paths.get(INDEX_FILENAME));
            } else {
                System.out.println("Parsing articles...");
                index = new SimpleFSDirectory(Paths.get(INDEX_FILENAME));
                final IndexWriter writer = new IndexWriter(index, config);
                Consumer<String> lineHandler = line -> handleLine(writer, line);

                // Parse the articles
                for(String directory : directories) {
                    try {
                        Files.list(Paths.get(directory)).forEach(f -> parseWikiDocument(lineHandler, f));
                    } catch(IOException ioe) {
                        System.err.printf("Error while attempting to parse directory(%s): %s\n",
                                directory, ioe);
                    }
                }

                // Close the index writer
                writer.close();
                System.out.println("Finished parsing articles.");
            }


            // Set up the query parser
            QueryParser queryParser = new QueryParser("contents", analyzer);

            // Read in user queries
            Scanner input = new Scanner(System.in);
            while(true) {
                System.out.print("Enter your query (exit to quit): ");
                String query = input.nextLine();

                if("exit".equals(query)) {
                    break;
                }

                query = query.toLowerCase().replaceAll("[^\\w\\s\\d\"]", "");

                try {
                    IndexReader reader = DirectoryReader.open(index);
                    IndexSearcher searcher = new IndexSearcher(reader);
                    //searcher.setSimilarity(new BM25Similarity());
                    TopScoreDocCollector collector = TopScoreDocCollector.create(HITS_PER_PAGE);
                    Query q = queryParser.parse(query);
                    searcher.search(q, collector);
                    ScoreDoc[] results = collector.topDocs().scoreDocs;

                    for(ScoreDoc doc : results) {
                        int docId = doc.doc;

                        Document d = searcher.doc(docId);
                        System.out.printf("Most likely: %s\n", d.get("article_title"));
                    }
                } catch(ParseException pe) {
                    System.err.printf("Error parsing query \"%s\"!\n", query);
                }
            }
        } catch(IOException ioe) {
            System.err.printf("Error openning directory: %s\n", "./watson_directory.dat");
        } catch(NoSuchElementException nsfe) {
            System.out.println();
            // Do nothing - END OF INPUT
        }
    }
}
