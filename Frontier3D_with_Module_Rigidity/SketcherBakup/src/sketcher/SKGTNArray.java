
/**
 * Title:        SKGTNArray
 * Description:  Array of SKGroupTreeNode
 */
   package sketcher;

   public class SKGTNArray extends BaseArray
   {
      protected SKGroupTreeNode[] baseArray;
   
      public SKGTNArray(int size, int growth) {
         super(size, growth, SKGroupTreeNode.class);
      }
   
      public SKGTNArray(int size) {
         super(size, SKGroupTreeNode.class);
      }
   
    // Implementation of callout to get the underlying array.
      protected Object getArray() {
         return baseArray;
      }
   
    // Implementation of callout to set the underlying array.
      protected void setArray(Object array) {
         baseArray = (SKGroupTreeNode[]) array;
      }
   
    // Implementation of callout to initialize a portion of the array.
      protected void discardValues(int from, int to) {
         for (int i = from; i < to; i++) {
            baseArray[i] = null;
         }
      }
   
    // Append a value to the collection.
      public int add(SKGroupTreeNode value) {
         int index = getAddIndex();
         baseArray[index] = value;
         return index;
      }
   
    // Insert a value into the collection.
      public void add(int index, SKGroupTreeNode value) {
         makeInsertSpace(index);
         baseArray[index] = value;
      }
   
    // Get value from the collection.
      public SKGroupTreeNode get(int index) {
         if (index < countPresent) {
            return baseArray[index];
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Set the value at a position in the collection.
      public void set(int index, SKGroupTreeNode value) {
         if (index < countPresent) {
            baseArray[index] = value;
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Convert to an array.
      public SKGroupTreeNode[] buildArray() {
         return (SKGroupTreeNode[]) buildArray(SKGroupTreeNode.class);
      }
   
      public int indexOf(Object ob)
      {
         int i;
         for (i=0; i<size(); i++)
         {
            if (get(i) == ob)
               return i;}

         return -1;
      }
   
      public void removeNode(SKGroupTreeNode sh)
      {
         remove( indexOf(sh) );
      }
   
   
      public SKGroupTreeNode findByID(int sID)
      
      {
         for (int i=0; i<size(); i++)
            if (get(i).ID==sID)  
               return get(i);
      
         return null;
      }
      public SKGroupTreeNode findByName(String s) 
       
      {
         for (int i=0; i<size(); i++)
            if (get(i).name.equals(s))
               return get(i);

         return null;
      }

   
   }
