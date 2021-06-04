// Nicklous J. Grant
// COP 3053, Summer 2019
// ni841669

import java.io.*;
import java.util.*;

class Node<T>
{

	private T data;
	private ArrayList<Node<T>> next;

	// Constructor that creates a node of the given height with all next
	// references set to null.
	// Mainly used to create the head of a SkipList.
	Node(int height)
	{

		next = new ArrayList<Node<T>>(height);
		for (int i = 0; i < height; i++)
		{
			next.add(null);
		}

	}

	// Same as above constructor with the only difference being
	// that the data field of the node is set.
	Node(T data, int height)
	{

		this.data = data;
		next = new ArrayList<Node<T>>(height);

		for (int i = 0; i < height; i++)
		{
			next.add(null);
		}

	}


	// Returns value of the node.
	public T value()
	{

		return data;

	}

	// Returns height of the node.
	public int height()
	{

		return next.size();

	}

	// Returns next node at the given level.
	// Range: 0 to (height - 1)
	// Will return null if level is outside of the range.
	public Node<T> next(int level)
	{

		if (level < 0 || next.size() <= level)
		{
			return null;
		}
		else
		{
			return next.get(level);
		}

	}

	// Adds a level to an exsisting node.
	// The new level's next reference is set to null.
	public void grow()
	{
		next.add(null);
	}

	// Functions similarly to the grow function above,
	// though it introduces a 50% probablity of actually
	// increasing the height of a node.
	// Returns 1 if the node grew and 0 otherwise.
	public int maybeGrow()
	{

		Random rand = new Random();

		if (rand.nextInt(2) == 1)
		{
			grow();
			return 1;
		}
		else
		{
			return 0;
		}

	}

	// Sets a node's next reference to the given node
	// at the given level.
	public void setNext(int level, Node<T> node)
	{
		next.set(level,node);
	}

	// Removes levels from a node until it's height is
	// equal to the height given.
	public void trim(int height)
	{
		for (int i = next.size(); i != height; i--)
		{
			next.remove(i-1);
		}
	}

}

public class SkipList<Type extends Comparable<Type>>
{

	private int size;
	private Node<Type> head;

	// Initializes the head of a SkipList with a height
	// of 1.
	SkipList()
	{
		size = 0;
		head = new Node<Type>(1);
	}

	// Initializes the head of a SkipList with the height
	// provided.
	SkipList(int height)
	{
		size = 0;
		head = new Node<Type>(height);
	}

	// Returns number of elements in SkipList.
	public int size()
	{
		return size;
	}

	// Returns height of SkipList.
	public int height()
	{
		return head.height();
	}

	// Returns the head of the SkipList
	public Node<Type> head()
	{
		return head;
	}



	// Calculates and returns the max suggested height of
	// a SkipList with the size given.
	private int getMaxHeight(int size)
	{
		double temp;

		temp = (Math.log((double)(size))) / (Math.log(2));
		temp = Math.ceil(temp);

		return (int)temp;
	}

	// Generates a random height for a node about to be inserted
	// into a SkipList based on the height of that list.
	private int getRandomHeight()
	{
		int genHeight = 1;
		Random rand = new Random();

		for (int i = 0; i < (head.height() - 1); i++)
		{

			if (rand.nextInt(2) == 1)
				genHeight++;
			else
				break;

		}

		return genHeight;
	}

	// Increases height of SkipList by one.
	// Procedure:
	// - Increase head node height by one
	// - Go through each node of SkipList (height - 1) with a
	//   50% probablity of growing the node
	private void growSkipList()
	{
		// Nodes of this level are the only ones that can grow.
		int canditateLevel = head.height() - 1;

		Node<Type> temp1 = head.next(canditateLevel);
		Node<Type> temp2 = head;

		// Grows head
		head.grow();

		for ( ; temp1 != null; temp1 = temp1.next(canditateLevel))
		{
			// If the node grew then we connect it to the
			// last node that was grown.
			if (temp1.maybeGrow() == 1)
			{
				temp2.setNext(head.height() - 1, temp1);
				temp2 = temp1;
			}
		}
	}

	// Decreases the height of SkipList to the
	// height given by the getMaxHeight fucntion.
	// Procedure:
	//  - Trim head node to suggested heights
	//  - Trim every node with hieght > maxHeight
	private void trimSkipList()
	{
		int temp = getMaxHeight(size);

		// The height of SkipList can never be less than one.
		if (temp < 1)
		{
			temp = 1;
		}

		Node<Type> temp1 = head;
		Node<Type> temp2 = head.next(temp);

		while(temp1.next(temp) != null)
		{
			temp1.trim(temp);

			temp1 = temp2;
			temp2 = temp1.next(temp);
		}

		temp1.trim(temp);
	}


	// Searches through a SkipList for the data given
	// and returns true if it is found and false otherwise.
	public boolean contains(Type data)
	{
		int currentLevel = head.height() - 1;
		Node<Type> temp1 = head;
		Node<Type> temp2 = head.next(currentLevel);

		while (currentLevel >= 0)
		{

			// Recahed the end of list on currentLevel,
			// So we go down and level and start again.
			if (temp2 == null)
			{
				currentLevel--;
				temp2 = temp1.next(currentLevel);
			}
			// We've found the data!!
			else if ((temp2.value()).compareTo(data) == 0)
			{
				return true;
			}
			// The given data is to the left of the current Node,
			// So we go down a level.
			else if ((temp2.value()).compareTo(data) > 0)
			{
				currentLevel--;
				temp2 = temp1.next(currentLevel);
			}
			// The given data is to the right of the current Node,
			// So we continue on to the next node at the currentLevel.
			else
			{
				temp1 = temp2;
				temp2 = temp1.next(currentLevel);
			}

		}

		return false; // Didn't find data.
	}

	// -- Searches through a SkipList for the data given and returns
	// the first instance of that data from the SkipList.
	// -- Returns Null if the data isn't found.
	// -- Functions very similarly to the contains function except for one
	// if condition in the main loop.
	public Node<Type> get(Type data)
	{
		int currentLevel = head.height() - 1;
		Node<Type> temp1 = head;
		Node<Type> temp2 = head.next(currentLevel);

		while(currentLevel >= 0)
		{

			// Recahed the end of list on currentLevel,
			// So we go down and level and start again.
			if (temp2 == null)
			{
				currentLevel--;
				temp2 = temp1.next(currentLevel);
			}
			// We've found the data!!
			else if ((temp2.value()).compareTo(data) == 0)
			{
				// This condition ensures that first instance(leftmost) of
				// the given data is returned from the SkipList.
				if (currentLevel == 0)
					return temp2;

				currentLevel--;
				temp2 = temp1.next(currentLevel);
			}
			// The given data is to the left of the current Node,
			// So we go down a level.
			else if((temp2.value()).compareTo(data) > 0)
			{
				currentLevel--;
				temp2 = temp1.next(currentLevel);
			}
			// The given data is to the right of the current Node,
			// So we continue on to the next node at the currentLevel.
			else
			{
				temp1 = temp2;
				temp2 = temp1.next(currentLevel);
			}

		}

		return null;
	}

	public void insert(Type data)
	{
		int currentLevel;

		Node<Type> temp1;
		Node<Type> temp2;
		Node<Type> insertionNode;

		ArrayList<Node<Type>> nextTemp;

		if (head.height() < getMaxHeight(++size))
			growSkipList();

		currentLevel = head.height() - 1;
		temp1 = head;
		temp2 = head.next(currentLevel);

		nextTemp = new ArrayList<Node<Type>>(head.height());
		for(int i = 0; i < head.height(); i++)
			nextTemp.add(null);

		while(currentLevel >= 0)
		{
			if (temp2 == null || (temp2.value()).compareTo(data) >= 0)
			{
				nextTemp.set(currentLevel, temp1);
				currentLevel--;
				temp2 = temp1.next(currentLevel);
			}
			else
			{
				temp1 = temp2;
				temp2 = temp1.next(currentLevel);
			}
		}

		insertionNode = new Node<Type>(data, getRandomHeight());

		for (int i = 0; i < insertionNode.height(); i++)
		{
			insertionNode.setNext(i, (nextTemp.get(i)).next(i));
			(nextTemp.get(i)).setNext(i, insertionNode);
		}
	}

	public void insert(Type data, int height)
	{
		int currentLevel;

		Node<Type> temp1;
		Node<Type> temp2;
		Node<Type> insertionNode;

		ArrayList<Node<Type>> nextTemp;

		if (head.height() < getMaxHeight(++size))
			growSkipList();

		currentLevel = head.height() - 1;
		temp1 = head;
		temp2 = head.next(currentLevel);

		nextTemp = new ArrayList<Node<Type>>(head.height());
		for(int i = 0; i < head.height(); i++)
			nextTemp.add(null);

		while(currentLevel >= 0)
		{

			if (temp2 == null || (temp2.value()).compareTo(data) >= 0)
			{
				nextTemp.set(currentLevel, temp1);
				currentLevel--;
				temp2 = temp1.next(currentLevel);
			}
			else
			{
				temp1 = temp2;
				temp2 = temp1.next(currentLevel);
			}

		}

		insertionNode = new Node<Type>(data, height);

		for (int i = 0; i < insertionNode.height(); i++)
		{

			insertionNode.setNext(i, (nextTemp.get(i)).next(i));
			(nextTemp.get(i)).setNext(i, insertionNode);

		}
	}

	public void delete(Type data)
	{
		int currentLevel;

		Node<Type> temp1;
		Node<Type> temp2;
		Node<Type> deletionNode;

		ArrayList<Node<Type>> nextTemp;
		
		if (!contains(data))
			return;

		if (head.height() > getMaxHeight(--size))
		{
			trimSkipList();
		}

		deletionNode = get(data);
		currentLevel = head.height() - 1;
		temp1 = head;
		temp2 = head.next(currentLevel);

		nextTemp = new ArrayList<Node<Type>>(head.height());
		for(int i = 0; i < head.height(); i++)
			nextTemp.add(null);

		while(currentLevel >= 0)
		{

			if (temp2 == null || (temp2.value()).compareTo(data) >= 0)
			{
				nextTemp.set(currentLevel, temp1);
				currentLevel--;
				temp2 = temp1.next(currentLevel);
			}
			else
			{
				temp1 = temp2;
				temp2 = temp1.next(currentLevel);
			}

		}

		for (int i = 0; i < deletionNode.height(); i++)
		{

			(nextTemp.get(i)).setNext(i, deletionNode.next(i));

		}

		// Effectively deleting node by destroying all referenes to
		// other nodes.
		deletionNode.trim(0);
	}

	// A function that prints the elemets in a skip list for
	// debugging purposes.
	public void printSkipList()
	{
		int i = 1;
		Node<Type> temp1 = head;
		Node<Type> temp2 = head.next(0);

		System.out.printf("SKIPLIST: Size(%d) Height(%d)\n", size, head.height());
		System.out.printf("Head: Height(%d)\n", head.height());

		for (; temp2 != null; temp2 = temp2.next(0))
		{
			System.out.printf("Node %d: Data(%s) Height(%d)\n", i, temp2.value() ,temp2.height());
			i++;
		}
	}



	// Statistical Functions
	public static double difficultyRating()
	{
		return 3.0;
	}
	public static double hoursSpent()
	{
		return 4.0;
	}

}
