
/**
 * Title:        SKShapeArray
 * Description:  Array of SKBaseShape
 */
   package sketcher;

   public class SKItemArray extends BaseArray
   {
      protected SKItemInterface[] baseArray;
   
      public SKItemArray(int size, int growth) {
         super(size, growth, SKItemInterface.class);
      }
   
      public SKItemArray(int size) {
         super(size, SKItemInterface.class);
      }
   
    // Implementation of callout to get the underlying array.
      protected Object getArray() {
         return baseArray;
      }
   
    // Implementation of callout to set the underlying array.
      protected void setArray(Object array) {
         baseArray = (SKItemInterface[]) array;
      }
   
    // Implementation of callout to initialize a portion of the array.
      protected void discardValues(int from, int to) {
         for (int i = from; i < to; i++) {
            baseArray[i] = null;
         }
      }
   
    // Append a value to the collection.
      public int add(SKItemInterface value) {
         int index = getAddIndex();
         baseArray[index] = value;
         return index;
      }
   
    // Insert a value into the collection.
      public void add(int index, SKItemInterface value) {
         makeInsertSpace(index);
         baseArray[index] = value;
      }
   
    // Get value from the collection.
      public SKItemInterface get(int index) {
         if (index < countPresent) {
            return baseArray[index];
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Set the value at a position in the collection.
      public void set(int index, SKItemInterface value) {
         if (index < countPresent) {
            baseArray[index] = value;
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Convert to an array.
      public SKItemInterface[] buildArray() {
         return (SKItemInterface[]) buildArray(SKItemInterface.class);
      }
   
      public int indexOf(Object ob)
      {
         int i;
         for (i=0; i<size(); i++)
            if (get(i) == ob)  
               return i;
      
         return -1;
      }
   
      public void remove(SKItemInterface sh)
      {  
         int i = indexOf(sh);
      
         if (i > -1)  remove(i);
      }
   }