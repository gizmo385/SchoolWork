import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import java.lang.reflect.Method;
import java.lang.reflect.InvocationTargetException;

import org.junit.Test;

/**
 * Test that tests adding 100,000 elements, might take a while to complete
 *
 * @author Jeremy Mowery
 *
 */
public class MasterTest {

    private boolean callValid( Tree tree ) {
        try {
            // Get the valid method from the tree class
            Method m = tree.getClass().getDeclaredMethod("valid");

            // Make it so we can invoke the method (break private access)
            m.setAccessible(true);

            return (Boolean)m.invoke(tree);
        }
        catch( NoSuchMethodException nsme ) {
            System.err.println("Could not find valid() in Tree class!");
            nsme.printStackTrace();
        }
        catch( IllegalAccessException iae ) {
            System.err.println("Illegal access reported while accessing valid():");
            iae.printStackTrace();
        }
        catch( InvocationTargetException ite ) {
            System.err.println("Valid() method threw an exception!");
            Throwable unwrappedException = ite.getCause();

            unwrappedException.printStackTrace();
        }

        return false;
    }

	@Test
	public void testForward() {
		Tree A = new Tree();
		Random random = new Random(300L);
		int min = 100001;
		int max = -100001;
		int oldMin = 100001;
		int oldMax = -100001;
		for (int i = 1; i <= 100000; i += 1) {
			int j = random.nextInt(i);
			A.insert(j, 2 * j);
			A.insert(j, 2 * j);
			assertEquals(A.find(j), j * 2);
			A.insert(j, 3 * j);
			assertEquals(3 * j, A.find(j));
			if (j > max) {
				assertTrue(j == A.max());
				oldMax = max;
				max = j;
				if (i > 2 && oldMax != -1) {
					assertTrue(A.prev(max) == oldMax);
					assertTrue(A.next(oldMax) == max);
					assertTrue(A.next(max) == null);
				}
			}
			if (j < min) {
				assertTrue(j == A.min());
				oldMin = min;
				min = j;
				if (i > 2 && oldMin != -1) {
					assertTrue(A.next(min) == oldMin);
					assertTrue(A.prev(oldMin) == min);
					assertTrue(A.prev(min) == null);
				}
			}
            callValid(A);

		}

		// Get the min from the tree and continuously call next until we get the
		// max
		min = A.min();
		max = A.max();
		int size = A.size();
		int index = 0;

		Integer next = A.next(min);
		int old = 0;
		while (next < max && index <= size) {
			old = next;
			next = A.next(next);
			index++;
			if (next < old) {
				fail("Next is not returning a larger value!");
			}
		}
		if (index > size || next != max) {
			fail("Did not get to the max");
		}
		Integer prev = A.prev(max);
		index = 0;
		while (prev > min && index <= size) {
			old = prev;
			prev = A.prev(prev);
			index++;
			if (prev > old) {
				fail("Prev is not returning a smaller element");
			}
		}
		if (index > size || prev != min) {
			fail("Did not get to the min");
		}

		System.out.println("****************************************");
		System.out.println("Remember to make valid and dump private again!");
		System.out.println("****************************************");
	}

	@Test
	public void testReverse() {
		Tree A = new Tree();
		Random random = new Random(300L);
		int min = 100001;
		int max = -100001;
		int oldMin = 100001;
		int oldMax = -100001;
		for (int i = 100000; i > 0; i -= 1) {
			int j = random.nextInt(i);
			A.insert(j, 2 * j);
			A.insert(j, 2 * j);
			assertEquals(A.find(j), j * 2);
			A.insert(j, 3 * j);
			assertEquals(3 * j, A.find(j));
			if (j > max) {
				assertTrue(j == A.max());
				oldMax = max;
				max = j;
				if (i > 2 && oldMax != -100001) {
					assertTrue(A.prev(max) == oldMax);
					assertTrue(A.next(oldMax) == max);
					assertTrue(A.next(max) == null);
				}
			}
			if (j < min) {
				assertTrue(j == A.min());
				oldMin = min;
				min = j;
				if (i > 2 && oldMin != 100001) {
					assertTrue(A.next(min) == oldMin);
					assertTrue(A.prev(oldMin) == min);
					assertTrue(A.prev(min) == null);
				}
			}
            assertTrue( callValid(A) );

		}

		// Get the min from the tree and continuously call next until we get the
		// max
		min = A.min();
		max = A.max();
		int size = A.size();
		int index = 0;

		Integer next = A.next(min);
		int old = 0;
		while (next < max && index <= size) {
			old = next;
			next = A.next(next);
			index++;
			if (next < old) {
				fail("Next is not returning a larger value!");
			}
		}
		if (index > size || next != max) {
			fail("Did not get to the max");
		}
		Integer prev = A.prev(max);
		index = 0;
		while (prev > min && index <= size) {
			old = prev;
			prev = A.prev(prev);
			index++;
			if (prev > old) {
				fail("Prev is not returning a smaller element");
			}
		}
		if (index > size || prev != min) {
			fail("Did not get to the min");
		}

		System.out.println("****************************************");
		System.out.println("Remember to make valid and dump private again!");
		System.out.println("****************************************");
	}

	@Test
	public void testMixForward() {
		Tree A = new Tree();
		Random random = new Random();
		int min = 100001;
		int max = -100001;
		int oldMin = 100001;
		int oldMax = -100001;
		for (int i = -100000; i <= 100000; i += 1) {
			int j = random.nextInt(Math.abs(i) + 1);
			if (i < 0) {
				j = -j;
			}
			A.insert(j, 2 * j);
			A.insert(j, 2 * j);
			assertEquals(A.find(j), j * 2);
			A.insert(j, 3 * j);
			assertEquals(3 * j, A.find(j));
			if (j > max) {
				assertTrue(j == A.max());
				oldMax = max;
				max = j;
				if (i > 2 && oldMax != -1) {
					assertTrue(A.prev(max) == oldMax);
					assertTrue(A.next(oldMax) == max);
					assertTrue(A.next(max) == null);
				}
			}
			if (j < min) {
				assertTrue(j == A.min());
				oldMin = min;
				min = j;
				if (i > 2 && oldMin != -1) {
					assertTrue(A.next(min) == oldMin);
					assertTrue(A.prev(oldMin) == min);
					assertTrue(A.prev(min) == null);
				}
			}
            assertTrue( callValid(A) );

		}

		// Get the min from the tree and continuously call next until we get the
		// max
		min = A.min();
		max = A.max();
		int size = A.size();
		int index = 0;

		Integer next = A.next(min);
		int old = 0;
		while (next < max && index <= size) {
			old = next;
			next = A.next(next);
			index++;
			if (next < old) {
				fail("Next is not returning a larger value!");
			}
		}
		if (index > size || next != max) {
			fail("Did not get to the max");
		}
		Integer prev = A.prev(max);
		index = 0;
		while (prev > min && index <= size) {
			old = prev;
			prev = A.prev(prev);
			index++;
			if (prev > old) {
				fail("Prev is not returning a smaller element");
			}
		}
		if (index > size || prev != min) {
			fail("Did not get to the min");
		}

		System.out.println("****************************************");
		System.out.println("Remember to make valid and dump private again!");
		System.out.println("****************************************");
	}

	@Test
	public void testMixBackward() {
		Tree A = new Tree();
		Random random = new Random();
		int min = 100001;
		int max = -100001;
		int oldMin = 100001;
		int oldMax = -100001;
		for (int i = 100000; i >= -100000; i -= 1) {
			int j = random.nextInt(Math.abs(i) + 1);
			if (i < 0) {
				j = -j;
			}
			A.insert(j, 2 * j);
			A.insert(j, 2 * j);
			assertEquals(A.find(j), j * 2);
			A.insert(j, 3 * j);
			assertEquals(3 * j, A.find(j));
			if (j > max) {
				assertTrue(j == A.max());
				oldMax = max;
				max = j;
				if (i > 2 && oldMax != -100001) {
					assertTrue(A.prev(max) == oldMax);
					assertTrue(A.next(oldMax) == max);
					assertTrue(A.next(max) == null);
				}
			}
			if (j < min) {
				assertTrue(j == A.min());
				oldMin = min;
				min = j;
				if (i > 2 && oldMin != 100001) {
					assertTrue(A.next(min) == oldMin);
					assertTrue(A.prev(oldMin) == min);
					assertTrue(A.prev(min) == null);
				}
			}
            assertTrue( callValid(A) );

		}

		// Get the min from the tree and continuously call next until we get the
		// max
		min = A.min();
		max = A.max();
		int size = A.size();
		int index = 0;

		Integer next = A.next(min);
		int old = 0;
		while (next < max && index <= size) {
			old = next;
			next = A.next(next);
			index++;
			if (next < old) {
				fail("Next is not returning a larger value!");
			}
		}
		if (index > size || next != max) {
			fail("Did not get to the max");
		}
		Integer prev = A.prev(max);
		index = 0;
		while (prev > min && index <= size) {
			old = prev;
			prev = A.prev(prev);
			index++;
			if (prev > old) {
				fail("Prev is not returning a smaller element");
			}
		}
		if (index > size || prev != min) {
			fail("Did not get to the min");
		}

		System.out.println("****************************************");
		System.out.println("Remember to make valid and dump private again!");
		System.out.println("****************************************");

		assertEquals(null, A.prev(-1000000000));
		assertEquals(null, A.next(1000000000));
		assertEquals(null, A.find(1000000000));
	}

	@Test
	public void prevAndNextAndMaxandMinTest() {
		Tree aTree = new Tree();
		aTree.insert(0, 10);
		assertEquals(null, aTree.prev(0));
		assertEquals(null, aTree.next(0));
		aTree.delete(0);
		assertEquals(null, aTree.max());
		assertEquals(null, aTree.min());
	}

	@Test
	public void testInsertByRemove() {
		Tree A = new Tree();
		Random random = new Random();
		List<Integer> elementsInTree = new ArrayList<>();
		for (int i = 100000; i >= -100000; i -= 1) {
			int j = random.nextInt(Math.abs(i) + 1);
			if (i < 0) {
				j = -j;
			}
			A.insert(j, i);
			if (!elementsInTree.contains(j)) {
				elementsInTree.add(j);
			}

            assertTrue( callValid(A) );

		}

		assertTrue(A.size() == elementsInTree.size());
		for (int i : elementsInTree) {
			A.delete(i);
            assertTrue( callValid(A) );

		}
		assertTrue(A.size() == 0);

		// Now do it backward
		elementsInTree.clear();
		for (int i = 100000; i >= -100000; i -= 1) {
			int j = random.nextInt(Math.abs(i) + 1);
			if (i < 0) {
				j = -j;
			}
			A.insert(j, i);
			elementsInTree.add(j);
            assertTrue( callValid(A) );

		}
		int size = elementsInTree.size();
		for (int i = size - 1; i >= 0; i--) {
			A.delete(elementsInTree.get(i));
            assertTrue( callValid(A) );
		}
		assertTrue(A.size() == 0);
	}
}
