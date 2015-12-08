package edu.arizona.cachapline8;

import java.nio.file.*;
import java.io.*;
import java.util.stream.*;
import java.util.function.*;
import java.util.regex.*;
import java.util.*;

import org.apache.lucene.analysis.standard.*;
import org.apache.lucene.util.*;
import org.apache.lucene.store.*;
import org.apache.lucene.index.*;
import org.apache.lucene.document.*;
import org.apache.lucene.queryparser.classic.*;
import org.apache.lucene.search.*;

public class Watson {

    private final static Pattern articleTitle = Pattern.compile("^\\[\\[(?<title>.+)\\]\\]$");
    private final static int HITS_PER_PAGE = 1;

    private static List<String> lines = new ArrayList<>();
    private static List<String> categories = new ArrayList<>();
    private static String currentArticleTitle = null;
    private static int numberOfArticles = 0;

    public static Document createDocument() {
        String contents = lines.stream().collect(Collectors.joining("\n"));
        String cats = categories.stream().collect(Collectors.joining(" "));

        Document d = new Document();
        d.add(new TextField("article_title", currentArticleTitle, Field.Store.YES));
        d.add(new TextField("contents", contents, Field.Store.NO));
        d.add(new TextField("categories", cats, Field.Store.NO));

        return d;
    }

    public static void handleLine(IndexWriter writer, String line) {
        Matcher m = articleTitle.matcher(line);
        if(m.matches()) {
            // A document out of the previous article
            if(currentArticleTitle != null) {
                Document d = createDocument();
                numberOfArticles++;

                try {
                    writer.addDocument(d);
                } catch(IOException ioe) {
                    System.err.printf("Error while writing document!\n");
                }
            }

            // Prepare for the next article
            lines.clear();
            categories.clear();
            currentArticleTitle = m.group("title");

        } else if(line.startsWith("CATEGORIES: ") ) {
            categories.add(line.substring(line.indexOf(':') + 1));
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
        StandardAnalyzer analyzer = new StandardAnalyzer();
        IndexWriterConfig config = new IndexWriterConfig(analyzer);

        System.out.println("Parsing articles...");

        try {
            Directory index = new SimpleFSDirectory(Paths.get("./watson_directory.dat"));
            final IndexWriter writer = new IndexWriter(index, config);
            Consumer<String> lineHandler = line -> handleLine(writer, line);

            // Parse the articles
            for(String directory : directories) {
                try {
                    Files.list(Paths.get(directory)).forEach(f -> parseWikiDocument(lineHandler, f));
                } catch(IOException ioe) {
                    System.err.printf("Error while attempting to parse directory(%s): %s\n", directory,
                            ioe);
                }
            }

            // Close the index writer
            writer.close();

            System.out.println("Finished parsing articles.");

            // Set up the query parser
            String[] fields = { "article_title", "contents", "categories" };
            Map<String, Float> boosts = new HashMap<>();
            boosts.put("article_title", 5f);
            boosts.put("contents", 2f);
            boosts.put("categories", 8f);
            MultiFieldQueryParser queryParser = new MultiFieldQueryParser(fields, analyzer);

            // Read in user queries
            Scanner input = new Scanner(System.in);
            while(true) {
                System.out.print("Enter your query: ");
                String query = input.nextLine();
                System.out.print("Enter the question category: ");
                String category = input.nextLine();

                if("exit".equals(query)) {
                    break;
                }

                String[] queries = {query, query, category};

                try {
                    IndexReader reader = DirectoryReader.open(index);
                    IndexSearcher searcher = new IndexSearcher(reader);
                    TopScoreDocCollector collector = TopScoreDocCollector.create(HITS_PER_PAGE);
                    Query q = queryParser.parse(queries, fields, analyzer);
                    searcher.search(q, collector);
                    ScoreDoc[] results = collector.topDocs().scoreDocs;

                    for(ScoreDoc doc : results) {
                        int docId = doc.doc;

                        Document d = searcher.doc(docId);
                        System.out.printf("Most likely: %s\n", d.get("article_title"));
                    }
                } catch(ParseException pe) {
                    System.err.printf("Error parsing query \"%s\": %s\n", query, pe);
                }
            }
        } catch(IOException ioe) {
            System.err.printf("Error openning directory: %s\n", "./watson_directory.dat");
        }
    }
}
