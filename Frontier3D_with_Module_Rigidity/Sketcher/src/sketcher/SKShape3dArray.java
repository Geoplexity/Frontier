/**
 * Title:        SKShapeArray
 * Description:  Array of SKBaseShape
 */
   package sketcher;

   public class SKShape3dArray extends BaseArray
   {
      protected SKBaseShape3d[] baseArray;
   
      public SKShape3dArray(int size, int growth) {
         super(size, growth, SKBaseShape3d.class);
      }
   
      public SKShape3dArray(int size) {
         super(size, SKBaseShape3d.class);
      }
   
    // Implementation of callout to get the underlying array.
      protected Object getArray() {
         return baseArray;
      }
   
    // Implementation of callout to set the underlying array.
      protected void setArray(Object array) {
         baseArray = (SKBaseShape3d[]) array;
      }
   
    // Implementation of callout to initialize a portion of the array.
      protected void discardValues(int from, int to) {
         for (int i = from; i < to; i++) {
            baseArray[i] = null;
         }
      }
   
    // Append a value to the collection.
      public int add(SKBaseShape3d value) {
         int index = getAddIndex();
         baseArray[index] = value;
         return index;
      }
   
    // Insert a value into the collection.
      public void add(int index, SKBaseShape3d value) {
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
/*      public void bubblesort()
      
      {
         for(int i=0; i<size(); i++)
            bubblemax(size() - i);
      }
   
   
      private void bubblemax(int eff_size)
      
      {
         for(int k=0; k<eff_size -1; k++)
         {
            if((baseArray[k].orientCons)<(baseArray[k+1].orientCons))
               swaparay(k, k+1);
            else if( (baseArray[k].orientCons)==(baseArray[k+1].orientCons)
                   &&(baseArray[k].ID) > (baseArray[k+1].ID) )
               swaparay(k, k+1);
         }
      }
   
*/   
      private void swaparay(int i, int j)
      
      {
         SKBaseShape3d temp=baseArray[i];
         baseArray[i]=baseArray[j];
         baseArray[j]=temp;
      }
   
      public int indexOf(SKBaseShape3d sh)
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
      public SKBaseShape3d get(int index) {
         if (index < countPresent) {
            return baseArray[index];
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Set the value at a position in the collection.
      public void set(int index, SKBaseShape3d value) {
         if (index < countPresent) {
            baseArray[index] = value;
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Convert to an array.
      public SKBaseShape3d[] buildArray() {
         return (SKBaseShape3d[]) buildArray(SKBaseShape3d.class);
      }
   
      public boolean removeShape(SKBaseShape3d sh)
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
   
      public SKBaseShape3d findByID(int sID)
      {
         for (int i=0; i<size(); i++)
            if (get(i).ID==sID)  
               return get(i);
      
         return null;
      }
   }
