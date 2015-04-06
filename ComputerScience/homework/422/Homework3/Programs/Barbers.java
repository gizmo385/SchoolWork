import java.util.List;
import java.util.ArrayList;

public class Barbers {

    private static Barber barber;
    private static int numberOfCustomers, arrivalInterval, haircutTime;

    public static void main( String[] args ) {
        if( args.length < 3 ) {
            System.err.println("Usage: java Barbers [numberOfCustomers] [haircutTime] [arrivalInterval]");
            System.exit(1);
        }

        // Read command-line arguments
        numberOfCustomers   = Integer.parseInt(args[0]);
        haircutTime         = Integer.parseInt(args[1]);
        arrivalInterval     = Integer.parseInt(args[2]);

        // Create the barber
        barber = new Barber();
        barber.start();

        // Create the customers
        for(int currentCustomer = 0; currentCustomer < numberOfCustomers; currentCustomer++ ) {
            Customer customer = new Customer(currentCustomer);
            customer.start();
        }
    }

    private static class Customer extends Thread {

        private int customerNumber, whenToArrive;
        public static Integer customerCount = 0;

        public Customer(int customerNumber) {
            this.customerNumber = customerNumber;
            this.whenToArrive = customerNumber * arrivalInterval;
        }

        public void run() {
            try {

                // Wait to arrive
                Thread.sleep(1000 * whenToArrive);
                System.out.printf("Customer %d has arrived!\n", customerNumber);

                synchronized(barber) {
                    // Wait for the barber to arrive
                    barber.wait();
                    System.out.printf("Customer %d is getting their haircut!\n", customerNumber);
                }
            } catch( InterruptedException ie ) {
                ie.printStackTrace();
            }
        }
    }

    private static class Barber extends Thread {

        public void run() {
            // Wait for a customer
            while(Customer.customerCount < numberOfCustomers) {
                synchronized(this) {
                    try {
                        // Wait for a customer
                        notify();

                        // Perform the haircut
                        Thread.sleep(haircutTime);
                    } catch( InterruptedException ie ) {
                        ie.printStackTrace();
                    }
                }
            }
        }
    }
}
