
/**
 * Title:        <p>
 * Description:  <p>
 * Copyright:    Copyright (c) <p>
 * Company:      <p>
 * @author
 * @version 1.0
 */
   package sketcher;

   public class SKConstraintArray extends BaseArray
   {
      protected SKBaseConstraint[] baseArray;
   
      public SKConstraintArray(int size, int growth) {
         super(size, growth, SKBaseConstraint.class);
      }
   
      public SKConstraintArray(int size) {
         super(size, SKBaseConstraint.class);
      }
   
    // Implementation of callout to get the underlying array.
      protected Object getArray() {
         return baseArray;
      }
   
    // Implementation of callout to set the underlying array.
      protected void setArray(Object array) {
         baseArray = (SKBaseConstraint[]) array;
      }
   
    // Implementation of callout to initialize a portion of the array.
      protected void discardValues(int from, int to) {
         for (int i = from; i < to; i++) {
            baseArray[i] = null;
         }
      }
   
    // Append a value to the collection.
      public int add(SKBaseConstraint value) {
         int index = getAddIndex();
         baseArray[index] = value;
         return index;
      }
   
    // Insert a value into the collection.
      public void add(int index, SKBaseConstraint value) {
         makeInsertSpace(index);
         baseArray[index] = value;
      }
   
    // Get value from the collection.
      public SKBaseConstraint get(int index) {
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
      public void set(int index, SKBaseConstraint value) {
         if (index < countPresent) {
            baseArray[index] = value;
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Convert to an array.
      public SKBaseConstraint[] buildArray() {
         return (SKBaseConstraint[]) buildArray(SKBaseConstraint.class);
      }
   
      public void remove(SKBaseConstraint con)
      {
         remove( indexOf(con) );
      }
   
   
      public SKBaseConstraint findByID(int sID)
      
      {
         for (int i=0; i<size(); i++)
            if (get(i).ID==sID)  
               return get(i);
      
         return null;
      }
   
   }