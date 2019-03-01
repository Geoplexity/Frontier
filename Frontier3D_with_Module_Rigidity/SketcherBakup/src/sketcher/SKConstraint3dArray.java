
/**
 * Title:        <p>
 * Description:  <p>
 * Copyright:    Copyright (c) <p>
 * Company:      <p>
 * @author
 * @version 1.0
 */
package sketcher;
   public class SKConstraint3dArray extends BaseArray
   {
      protected SKBaseConstraint3d[] baseArray;
   
      public SKConstraint3dArray(int size, int growth) {
         super(size, growth, SKBaseConstraint3d.class);
      }
   
      public SKConstraint3dArray(int size) {
         super(size, SKBaseConstraint3d.class);
      }
   
    // Implementation of callout to get the underlying array.
      protected Object getArray() {
         return baseArray;
      }
   
    // Implementation of callout to set the underlying array.
      protected void setArray(Object array) {
         baseArray = (SKBaseConstraint3d[]) array;
      }
   
    // Implementation of callout to initialize a portion of the array.
      protected void discardValues(int from, int to) {
         for (int i = from; i < to; i++) {
            baseArray[i] = null;
         }
      }
   
    // Append a value to the collection.
      public int add(SKBaseConstraint3d value) {
         int index = getAddIndex();
         baseArray[index] = value;
         return index;
      }
   
    // Insert a value into the collection.
      public void add(int index, SKBaseConstraint3d value) {
         makeInsertSpace(index);
         baseArray[index] = value;
      }
   
    // Get value from the collection.
      public SKBaseConstraint3d get(int index) {
         if (index < countPresent) {
            return baseArray[index];
         }
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
      public int indexOf(Object ob)
      {
         int i;
         for (i=0; i<size(); i++)
            if (get(i) == ob)  
               return i;
      
         return -1;
      }
    // Set the value at a position in the collection.
      public void set(int index, SKBaseConstraint3d value) {
         if (index < countPresent) {
            baseArray[index] = value;
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Convert to an array.
      public SKBaseConstraint3d[] buildArray() {
         return (SKBaseConstraint3d[]) buildArray(SKBaseConstraint3d.class);
      }
   
      public void remove(SKBaseConstraint3d con)
      {
         remove( indexOf(con) );
      }
   
   
      public SKBaseConstraint3d findByID(int sID)
      
      {
         for (int i=0; i<size(); i++)
            if (get(i).ID==sID)  
               return get(i);
      
         return null;
      }
   
   }
