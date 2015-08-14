import java.util.List;
import java.util.ArrayList;

public class Barbers {

    private static Barber theBarber;
    private static int numberOfCustomers, arrivalInterval, haircutTime;

    // Counters
    private static int barber = 0, chair = 0, open = 0;

    // Condition variables
    private static Object barberAvailable = new Object();
    private static Object chairOccupied = new Object();
    private static Object doorOpen = new Object();
    private static Object customerLeft = new Object();

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
        theBarber = new Barber();
        theBarber.start();

        // Create the customers
        for(int currentCustomer = 0; currentCustomer < numberOfCustomers; currentCustomer++ ) {
            Customer customer = new Customer(currentCustomer);
            customer.start();
        }
    }

    private static void getHaircut( int customerId) throws InterruptedException {
        System.out.printf("Customer %d has arrived at the barber shop!\n", customerId);

        // Wait for the barber to be available
        while( barber == 0 ) {
            synchronized(barberAvailable) {
                barberAvailable.wait();
            }
        }

        // Sit down for the haircut
        barber--;

        chair++;
        System.out.printf("Customer %d has started their haircut!\n", customerId);

        // Simulate the haircut time
        Thread.sleep(haircutTime * 1000);

        System.out.printf("Customer %d has finished their haircut!\n", customerId);

        // Leave the chair
        synchronized(chairOccupied) {
            chairOccupied.notify();
        }

        // Wait for the door to be opened
        while(open == 0) {
            synchronized(doorOpen) {
                doorOpen.wait();
            }
        }

        // Leave the establishment
        open--;
        synchronized(customerLeft) {
            customerLeft.notify();
        }
        System.out.printf("Customer %d has left the building!\n", customerId);
    }

    private static void getNextCustomer()  throws InterruptedException {
        barber++;

        // Notify the next customer
        synchronized(barberAvailable) {
            barberAvailable.notify();
        }

        // Wait for the customer to sit down
        while( chair == 0 ) {
            synchronized(chairOccupied) {
                chairOccupied.wait();
            }
        }

        chair--;
    }

    private static void finishedCut() throws InterruptedException {
        open++;
        synchronized(doorOpen) {
            doorOpen.notify();
        }

        while( open > 0 ) {
            synchronized(customerLeft) {
                customerLeft.wait();
            }
        }
    }

    private static class Customer extends Thread {

        private int customerId;

        public Customer(int customerId) {
            this.customerId = customerId;
        }

        public void run() {
            try {
                Thread.sleep(customerId * arrivalInterval * 1000);
                getHaircut(customerId);
            } catch( InterruptedException ie ) {
                ie.printStackTrace();
            }
        }
    }

    private static class Barber extends Thread {
        public void run() {

            System.out.printf("The barber shop has opened for the day! Serving %d customers today!\n",
                    numberOfCustomers);

            for(int customersHandled = 0; customersHandled < numberOfCustomers; customersHandled++) {
                try {
                    getNextCustomer();
                    finishedCut();
                } catch( InterruptedException ie ) {
                    ie.printStackTrace();
                }
            }

            System.out.println("The barber shop is closing for the day!");
        }
    }
}
