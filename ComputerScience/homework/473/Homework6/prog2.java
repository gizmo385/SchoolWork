import java.io.*;

public class prog2 {
    static int lookahead;

    public prog2() throws IOException {
        lookahead = System.in.read();
    }

    void E() throws IOException {

        if( lookahead == '(' ) {
            System.out.println("E -> (E)");
            match('(');
            E();
            match(')');
        } else {
            System.out.println("E -> TR");
            T();

            for(;;) {

                if( lookahead == '*' ) {
                    System.out.println("R -> *TR");
                    match('*');
                    T();
                    continue;
                } else if( lookahead == '/' ) {
                    System.out.println("R -> /TR");
                    match('/');
                    T();
                    continue;
                }

                break;
            }

            System.out.println("R -> A");
            A();
        }
    }

    void A() throws IOException {
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

        System.out.println("A -> epsilon" );
    }

    void T() throws IOException {
        if( Character.isDigit((char)lookahead) ) {
            System.out.printf("T -> %c\n", (char)lookahead);
            match(lookahead);
        } else if (lookahead == '(' ) {
            match('(');
            E();
            match(')');
        } else {
            throw new Error("syntax error");
        }
    }

    void match(int t) throws IOException {
        if( lookahead == t )
            lookahead = System.in.read();
        else throw new Error("syntax error");
    }

    public static void main(String[] args) throws IOException {
        prog2 parse = new prog2();
        parse.E();
    }
}
