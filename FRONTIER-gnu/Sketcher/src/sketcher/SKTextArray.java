/**
 * Title:        SKShapeArray
 * Description:  Array of SKText
 */
   package sketcher;

   public class SKTextArray extends BaseArray
   {
      protected SKText[] baseArray;
   
      public SKTextArray(int size, int growth) {
         super(size, growth, SKText.class);
      }
   
      public SKTextArray(int size) {
         super(size, SKText.class);
      }
   
    // Implementation of callout to get the underlying array.
      protected Object getArray() {
         return baseArray;
      }
   
    // Implementation of callout to set the underlying array.
      protected void setArray(Object array) {
         baseArray = (SKText[]) array;
      }
   
    // Implementation of callout to initialize a portion of the array.
      protected void discardValues(int from, int to) {
         for (int i = from; i < to; i++) {
            baseArray[i] = null;
         }
      }
   
    // Append a value to the collection.
      public int add(SKText value) {
         int index = getAddIndex();
         baseArray[index] = value;
         return index;
      }
   
    // Insert a value into the collection.
      public void add(int index, SKText value) {
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
   
      public int indexOf(SKText sh)
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
      public SKText get(int index) {
         if (index < countPresent) {
            return baseArray[index];
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Set the value at a position in the collection.
      public void set(int index, SKText value) {
         if (index < countPresent) {
            baseArray[index] = value;
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Convert to an array.
      public SKText[] buildArray() {
         return (SKText[]) buildArray(SKText.class);
      }
   
      public boolean removeText(SKText sh)
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
      public SKText findByValue(String s)
      {
         for (int i=0; i<size(); i++)
            if (get(i).value.equals(s))
               return get(i);

         return null;
      }
   
   }
