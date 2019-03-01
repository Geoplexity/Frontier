/**
 * Title:        SKBifurcationArray
 * Description:  Array of SKBifurcation
 */
   package sketcher;
   import java.io.File;
   import java.io.BufferedReader;
   import java.io.FileReader;
   import java.io.IOException;
   import java.awt.Container;
   import java.awt.Container;

   public class SKBifurcationArray extends BaseArray
   {
      public SKShape3dArray shapes;  //Array of shapes involved in Bifurcations
      public int[]   startIndices; //Beginning indices into SKBifurcation.data
      protected SKBifurcation[] baseArray;
   
      public SKBifurcationArray(int size, int growth) {
         super(size, growth, SKBifurcation.class);
      }
   
      public SKBifurcationArray(int size) {
         super(size, SKBifurcation.class);
      }
   
    // Implementation of callout to get the underlying array.
      protected Object getArray() {
         return baseArray;
      }
   
    // Implementation of callout to set the underlying array.
      protected void setArray(Object array) {
         baseArray = (SKBifurcation[]) array;
      }
   
    // Implementation of callout to initialize a portion of the array.
      protected void discardValues(int from, int to) {
         for (int i = from; i < to; i++) {
            baseArray[i] = null;
         }
      }
   
    // Append a value to the collection.
      public int add(SKBifurcation value) {
         int index = getAddIndex();
         baseArray[index] = value;
         return index;
      }
   
    // Insert a value into the collection.
      public void add(int index, SKBifurcation value) {
         makeInsertSpace(index);
         baseArray[index] = value;
      }
   
    // Get value from the collection.
      public SKBifurcation get(int index) {
         if (index < countPresent) {
            return baseArray[index];
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Set the value at a position in the collection.
      public void set(int index, SKBifurcation value) {
         if (index < countPresent) {
            baseArray[index] = value;
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Convert to an array.
      public SKBifurcation[] buildArray()
      {
         return (SKBifurcation[]) buildArray(SKBifurcation.class);
      }
   
      public int indexOf(Object ob)
      {
         int i;
         for (i=0; i<size() && get(i) != ob; i++)
         {}
         return i;
      }
   
      public void remove(SKBifurcation sh)
      {
         remove( indexOf(sh) );
      }
   /**
   *Draws the different bifurcations of the shapes on the bifurcation window
   */
      public void updateShapes(int bifurIndex, SKBifurFrame frame)
      {
         SKBifurcation bifur = get(bifurIndex);
         boolean needBifurValues = (bifur.scale == -1);
         if (needBifurValues)
         {
            bifur.addX = frame.pnlShapes.getWidth()>>1;
            bifur.addY = frame.pnlShapes.getHeight()>>1;
            bifur.scale = 40;
         }
         for(int i=0; i<startIndices.length; i++)
         {
            shapes.get(i).bifurUpdate(bifur.data,startIndices[i],bifur.addX,bifur.addY,bifur.scale);
         }
         if (needBifurValues)
         {
         //Update sketch so it fits on screen
  	  /* double minX, minY, maxX, maxY, minZ, maxZ;
           maxX = -999;
           minX = 999;
           maxY = -999;
           minY = 999;
           minZ = 999;
           maxZ = -999;

	   for(int i=0; i<shapes.size(); i++)
            for(int j=1; j<shapes.get(i).getNumSubShapes()+1;j++)
            {
             SKBaseShape3d shape = shapes.get(i).getSubShape(j);
             if(minX>shape.getDX()) minX = shape.getDX();
             if(maxX<shape.getDX()) maxX = shape.getDX();
             if(minY>shape.getDY()) minY = shape.getDY();
             if(maxY<shape.getDY()) maxY = shape.getDY();
             if(minZ>shape.getDZ()) minZ = shape.getDZ();
             if(maxZ<shape.getDZ()) maxZ = shape.getDZ();
	     System.out.println("111 "+shape.getDX()+" "+shape.getDY()+" "+shape.getDZ());
            }
            double[] minMax = new double[6];
            minMax[0] = 999;
            minMax[1] = 999;
            minMax[2] = 999;
            minMax[3] = -999;
            minMax[4] = -999; 
            minMax[5] = -999;
            //SKUTUFile.centralize(minX, minY, minZ, maxX, maxY, maxZ, shapes, minMax);
            minX = minMax[0];
            minY = minMax[1];
            minZ = minMax[2];
            maxX = minMax[3];
            maxY = minMax[4];
            maxZ = minMax[5];
            SKUTUFile.doScale(shapes);
	    System.out.println("111 doscale");         */
         
            updateShapes(bifurIndex,frame);
         }
      }
   		/**
   *Reads the coordinates and the other properties of the shapes from the arrays sent by the solver 
   */
      public void readFromStream(int dataInt[], double dataDouble[], int index[],  int numBifur, SKShape3dArray allShapes, SKMainFrame frame, Container shapeArea, SKBifurFrame dlg)
      {
         shapes = new SKShape3dArray(10);
         int indexInt = index[0], indexDbl = index[1];    
         //Read data
         SKBifurcation bifur;
         SKBaseShape3d sh,newSh;
         int cnt=dataInt[indexInt++],i,j,k,id,pos;
         startIndices = new int[cnt];
	 //System.out.println("Number of shapes in bifurcation.... " + cnt);
	 //stdio.getLine();
         for (i=0; i<numBifur; i++)
         {   
            bifur = new SKBifurcation();
            frame.RefreshShapeArea();
            bifur.data = new float[ 5*cnt ]; //need a better way
            pos = 0;
            for (j=0; j<cnt; j++)
            {

               id = dataInt[indexInt++];
            
               if (i == 0)
               {sh = allShapes.findByID(id);
                  switch (sh.ShapeTypeID)
                  {
                     case 6:  newSh = new SKPointShape3d(frame,sh.ID,sh.getX(),sh.getY(),0);
			      newSh.changeColor(((SKPointShape3d)sh).color);
			      //System.out.println("Found a point Shape... ");
			      //stdio.getLine();
                        break;
                     case 7:  newSh = new SKLineShape3d(frame,sh.ID,sh.getX(),sh.getY(),0);
                        break;
                     default: newSh = null;
                  }
               
                  if (newSh != null)
                  {
                     //System.out.println("array"+newSh);
                     for(int l=1; l<sh.getNumSubShapes()+1; l++)
                     {
                        sh.getSubShape(l).cloneShape(newSh.getSubShape(l));
			((JSelectionPanel3d)shapeArea).add(newSh.getSubShape(l));
                     }
		     dlg.Shapes.add(newSh);
  		     if(sh instanceof SKPointShape3d)
		     {   ((SKPointShape3d)newSh).changeRadius(((SKPointShape3d)sh).radius);
			 ((SKPointShape3d)newSh).radius = ((SKPointShape3d)sh).radius;
		     }
                     shapeArea.repaint();
                     startIndices[ shapes.add( newSh ) ] = pos;
                  }
               }
               else
                  sh = shapes.findByID(id);
               for (k=0; k<sh.bifurDataCount(); k++)
               {  bifur.data[pos] = (float)dataDouble[indexDbl++];
                  pos++;
               }
            }
            add(bifur);
         }
	 dlg.cloneConstraints(dlg.Shapes);
         index[0]= indexInt;
         index[1]= indexDbl;
      }
   }
