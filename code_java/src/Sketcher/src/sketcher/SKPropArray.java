
/**
 * Title:        SKPropArray
 * Description:  Array of SKProps
 */
   package sketcher;

   public class SKPropArray extends BaseArray
   {
      protected SKProp[] baseArray;
   
      public SKPropArray(int size, int growth) {
         super(size, growth, SKProp.class);
      }
   
      public SKPropArray(int size) {
         super(size, SKProp.class);
      }
   
    // Implementation of callout to get the underlying array.
      protected Object getArray() {
         return baseArray;
      }
   
    // Implementation of callout to set the underlying array.
      protected void setArray(Object array) {
         baseArray = (SKProp[]) array;
      }
   
    // Implementation of callout to initialize a portion of the array.
      protected void discardValues(int from, int to) {
         for (int i = from; i < to; i++) {
            baseArray[i] = null;
         }
      }
   
    // Append a value to the collection.
      public int add(SKProp value) {
         int index = getAddIndex();
         baseArray[index] = value;
         return index;
      }
   
    // Insert a value into the collection.
      public void add(int index, SKProp value) {
         makeInsertSpace(index);
         baseArray[index] = value;
      }
   
    // Get value from the collection.
      public SKProp get(int index) {
         if (index < countPresent) {
            return baseArray[index];
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Set the value at a position in the collection.
      public void set(int index, SKProp value) {
         if (index < countPresent) {
            baseArray[index] = value;
         }
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Convert to an array.
      public SKProp[] buildArray() {
         return (SKProp[]) buildArray(SKProp.class);
      }
   
      public String getShapePropName(int index)
      {
         return get(index).PropName;
      }
   
      public Object getShapePropData(int index)
      {
         return get(index).PropData;
      }
   
      public int getShapePropType(int index)
      {
         return get(index).PropType;
      }
   
      public void setShapePropData(int index, Object data)
      {
         get(index).PropData = data;
      }
   }