/**
 * Title:        SKShapeArray
 * Description:  Array of SKClusterObj
 */
   package sketcher;

   public class SKClusterObjArray extends BaseArray
   {
      protected SKClusterObj[] baseArray;
   
      public SKClusterObjArray(int size, int growth) {
         super(size, growth, SKClusterObj.class);
      }
   
      public SKClusterObjArray(int size) {
         super(size, SKClusterObj.class);
      }
   
    // Implementation of callout to get the underlying array.
      protected Object getArray() {
         return baseArray;
      }
   
    // Implementation of callout to set the underlying array.
      protected void setArray(Object array) {
         baseArray = (SKClusterObj[]) array;
      }
   
    // Implementation of callout to initialize a portion of the array.
      protected void discardValues(int from, int to) {
         for (int i = from; i < to; i++) {
            baseArray[i] = null;
         }
      }
   
    // Append a value to the collection.
      public int add(SKClusterObj value) {
         int index = getAddIndex();
         baseArray[index] = value;
         return index;
      }
   
    // Insert a value into the collection.
      public void add(int index, SKClusterObj value) {
         makeInsertSpace(index);
         baseArray[index] = value;
      }
   
      public int indexOf(Object ob)
      {//System.out.println("entered IndexOf");
         int i;
         for (i=0; i<size(); i++)
         {//System.out.println(get(i)+"after this comes ob");
            if (get(i) == ob)  
               return i;}
         return -1;
      }
   
      public int indexOf(SKClusterObj sh)
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
      public SKClusterObj get(int index) {
         if (index < countPresent) {
            return baseArray[index];
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Set the value at a position in the collection.
      public void set(int index, SKClusterObj value) {
         if (index < countPresent) {
            baseArray[index] = value;
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Convert to an array.
      public SKClusterObj[] buildArray() {
         return (SKClusterObj[]) buildArray(SKClusterObj.class);
      }
   
      public boolean removeClusterObj(SKClusterObj sh)
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
      public SKClusterObj findByID(int ind)
      {
         for (int i=0; i<size(); i++)
            if (get(i).ID == ind)
               return get(i);
      
         return null;
      }
   
   }
