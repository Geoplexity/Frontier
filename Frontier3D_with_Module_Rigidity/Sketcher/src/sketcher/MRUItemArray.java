/**
 * Title:        MRUItemArray
 * Description:  Array of MRUItems
 */
   package sketcher;

   public class MRUItemArray extends BaseArray
   {
      protected MRUItem[] baseArray;
   
      public MRUItemArray(int size, int growth)
      {
         super(size, growth, MRUItem.class);
      }
   
      public MRUItemArray(int size)
      {
         super(size, MRUItem.class);
      }
   
    // Implementation of callout to get the underlying array.
      protected Object getArray() {
         return baseArray;
      }
   
    // Implementation of callout to set the underlying array.
      protected void setArray(Object array) {
         baseArray = (MRUItem[]) array;
      }
   
    // Implementation of callout to initialize a portion of the array.
      protected void discardValues(int from, int to) {
         for (int i = from; i < to; i++) {
            baseArray[i] = null;
         }
      }
   
    // Append a value to the collection.
      public int add(MRUItem value)
      {
        //Dupe?
         for (int i=0; i<countPresent; i++)
            if (value.displayText.equalsIgnoreCase(baseArray[i].displayText))
            {
               remove(i);
               add(0,value);
               return 0;
            }
      
         int index = getAddIndex();
         baseArray[index] = value;
         return index;
      }
   
    // Insert a value into the collection.
      public void add(int index, MRUItem value) {
         makeInsertSpace(index);
         baseArray[index] = value;
      }
   
    // Get value from the collection.
      public MRUItem get(int index) {
         if (index < countPresent) {
            return baseArray[index];
         }
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Set the value at a position in the collection.
      public void set(int index, MRUItem value) {
         if (index < countPresent) {
            baseArray[index] = value;
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Convert to an array.
      public MRUItem[] buildArray() {
         return (MRUItem[]) buildArray(MRUItem.class);
      }
   
      public int indexOf(Object ob)
      {
         int i;
         for (i=0; i<size() && get(i) != ob; i++)
         {}
      
         return i;
      }
   
      public void remove(MRUItem sh)
      {
         remove( indexOf(sh) );
      }
   }