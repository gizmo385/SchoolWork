import java.io.*;

class Parser {
    static int lookahead;

    public Parser() throws IOException {
        lookahead = System.in.read();
    }

    void E() throws IOException {
        System.out.println("E -> TR");
        T();

        for(;;) {

            if( lookahead == '+' ) {
                System.out.println("R -> +TR");
                match('+');
                T();
                continue;
            } else if( lookahead == '-' ) {
                System.out.println("R -> -TR");
                match('-');
                T();
                continue;
            }

            break;
        }

        System.out.println("T -> epsilon");
    }

    void T() throws IOException {
        if( Character.isDigit((char)lookahead) ) {
            System.out.printf("T -> %c\n", (char)lookahead);
            match(lookahead);
        } else throw new Error("syntax error");
    }

    void match(int t) throws IOException {
        if( lookahead == t )
        lookahead = System.in.read();
        else throw new Error("syntax error");
    }
}
