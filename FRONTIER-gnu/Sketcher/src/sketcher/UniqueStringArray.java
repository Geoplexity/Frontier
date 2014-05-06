/**
 * Title:        UniqueStringArray
 * Description:  Array of unique strings
 */
   package sketcher;

   public class UniqueStringArray extends BaseArray
   {
      protected String[] baseArray;
   
      public UniqueStringArray(int size, int growth) {
         super(size, growth, String.class);
      }
   
      public UniqueStringArray(int size) {
         super(size, String.class);
      }
   
    // Implementation of callout to get the underlying array.
      protected Object getArray() {
         return baseArray;
      }
   
    // Implementation of callout to set the underlying array.
      protected void setArray(Object array) {
         baseArray = (String[]) array;
      }
   
    // Implementation of callout to initialize a portion of the array.
      protected void discardValues(int from, int to) {
         for (int i = from; i < to; i++) {
            baseArray[i] = null;
         }
      }
   
    // Append a value to the collection.
      public int add(String value) {
        //Only add unique values
         for (int i=0; i<countPresent; i++)
            if (get(i).equals(value)) 
               return -1;
      
         int index = getAddIndex();
         baseArray[index] = value;
         return index;
      }
   
    // Append multiple values
      public void add(String[] values)
      {
         for (int i=0; i<values.length; i++)
            add(values[i]);
      }
   
    // Insert a value into the collection.
      public void add(int index, String value) {
        //Only add unique values
         for (int i=0; i<countPresent; i++)
            if (get(i).equals(value)) 
               return;
      
         makeInsertSpace(index);
         baseArray[index] = value;
      }
   
    // Get value from the collection.
      public String get(int index) {
         if (index < countPresent) {
            return baseArray[index];
         }
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Set the value at a position in the collection.
      public void set(int index, String value) {
         if (index < countPresent) {
            baseArray[index] = value;
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Convert to an array.
      public String[] buildArray() {
         return (String[]) buildArray(String.class);
      }
   
      public int indexOf(Object ob)
      {
         int i;
         for (i=0; i<size() && get(i) != ob; i++)
         {}
      
         return i;
      }
   
      public void remove(String sh)
      {
         remove( indexOf(sh) );
      }
   }