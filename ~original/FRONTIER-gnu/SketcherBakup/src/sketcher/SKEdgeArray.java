/**
 * Title:        SKShapeArray
 * Description:  Array of SKEdge
 */
   package sketcher;

   public class SKEdgeArray extends BaseArray
   {
      protected SKEdge[] baseArray;
   
      public SKEdgeArray(int size, int growth) {
         super(size, growth, SKEdge.class);
      }
   
      public SKEdgeArray(int size) {
         super(size, SKEdge.class);
      }
   
    // Implementation of callout to get the underlying array.
      protected Object getArray() {
         return baseArray;
      }
   
    // Implementation of callout to set the underlying array.
      protected void setArray(Object array) {
         baseArray = (SKEdge[]) array;
      }
   
    // Implementation of callout to initialize a portion of the array.
      protected void discardValues(int from, int to) {
         for (int i = from; i < to; i++) {
            baseArray[i] = null;
         }
      }
   
    // Append a value to the collection.
      public int add(SKEdge value) {
         int index = getAddIndex();
         baseArray[index] = value;
         return index;
      }
   
    // Insert a value into the collection.
      public void add(int index, SKEdge value) {
         makeInsertSpace(index);
         baseArray[index] = value;
      }
   
      public int indexOf(Object ob)
      {//System.out.println("entered IndexOf");
         int i;
         //System.out.println(size());
         for (i=0; i<size(); i++)
         {//System.out.println(get(i)+"after this comes ob");
           // System.out.println(ob);
            if (get(i) == ob)  
               return i;}
      
      
         return -1;
      }
   
      public int indexOf(SKEdge sh)
      {//System.out.println("entered IndexOf");
         int i;
         //System.out.println(size());
         for (i=0; i<size(); i++)
         {//System.out.println(get(i)+"after this comes"+ sh);
            if (get(i) == sh)  
               return i;}
      
      
         return -1;
      }
   
   
   
    // Get value from the collection.
      public SKEdge get(int index) {
         if (index < countPresent) {
            return baseArray[index];
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Set the value at a position in the collection.
      public void set(int index, SKEdge value) {
         if (index < countPresent) {
            baseArray[index] = value;
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Convert to an array.
      public SKEdge[] buildArray() {
         return (SKEdge[]) buildArray(SKEdge.class);
      }
   
      public boolean removeEdge(SKEdge sh)
      {
         int i = indexOf(sh);
         if (i != -1)
         {
            remove(i);
            return true;
         }
         else 
            return false;
      }
   
   }
