/**
 * Title:        SKGroupTreeNode
 * Description:  Represents the shape groups tree
 */
   package sketcher;

   import java.io.*;

   public class SKGroupTreeNode
   {
      public int              fixed=0;
      public int              width=500;
      public int              ID;
      public int	      level=0;
      public int 	      overConst = 0; //only relevent for DAG clusters.
      public String           name;
      public String           matchingIP="";
      public SKGroupTreeNode  parent;
      public SKGTNArray       parents= new SKGTNArray(100);
      public SKShapeArray     shapes   = new SKShapeArray(2);
      public SKGTNArray       children = new SKGTNArray(2);
      public IBox             groupRect = new IBox(0,0,0,0);
      public int              x=0,y=0;
      private SKGTNArray temp = new SKGTNArray(2);
      public SKGroupTreeNode(SKGroupTreeNode aParent, int aID, String aName)
      {
         parent = aParent;
         ID = aID;
         name = aName;
      }
   
      public void removeChildren()
      {
         for (int i=0; i<children.size(); i++)
            children.get(i).removeChildren();
      
         children.clear();
      }
   
      public int saveToUTUFile(int indexInt, int intArray[])
      {
         if(name.charAt(1)=='s')
         {
            for (int i=0; i<children.size(); i++) 
            {
               indexInt=children.get(i).saveToUTUFile(indexInt, intArray);
            }
         }
         else
         {
            intArray[indexInt++] = ID;
            SKShapeArray tempShapes = new SKShapeArray(2);
            shapes.copyArrayTo(tempShapes);
            for(int j=0; j<shapes.size(); j++)
            { 
               boolean remove=false;
               for(int i=0; i<children.size(); i++)
                  remove = remove || ((children.get(i)).shapes.indexOf(shapes.get(j))!=-1);
               if(remove)
                  tempShapes.removeShape(shapes.get(j));
            }
            intArray[indexInt++] = tempShapes.size();
            int i;
            for (i=0; i<tempShapes.size(); i++)
               intArray[indexInt++] = tempShapes.get(i).ID;
	    if(ID !=-1)
               intArray[indexInt++] = children.size();
	    else
	    {
		int count=0;
		for(i=0; i<children.size(); i++)
		  count= count + children.get(i).children.size();
		intArray[indexInt++] = count;	
	    }
            for (i=0; i<children.size(); i++) 
               indexInt=children.get(i).saveToUTUFile(indexInt, intArray);
         
         }
         return indexInt;
      }
   
      public void saveToStream(DataOutputStream p) throws IOException
      {
         p.writeInt(ID);
         p.writeUTF(name);
      
         p.writeInt(shapes.size());
         int i;
         for (i=0; i<shapes.size(); i++)
            p.writeInt(shapes.get(i).ID);
      
         p.writeInt(children.size());
         for (i=0; i<children.size(); i++)
            children.get(i).saveToStream(p);
      }
   
      public void readFromStream(DataInputStream p, JSelectionPanel pnl) throws IOException
      {
         ID = p.readInt();
         name = p.readUTF();
      
      //Read and associate shapes
         int i,
         cnt = p.readInt();
         int[] IDlist = new int[cnt];
         for (i=0; i<cnt; i++)
            IDlist[i] = p.readInt();
      
         for (int j=0; j<cnt; j++)
         {
            i=0;
            while (((SKBaseShape)pnl.getComponent(i)).getSelectable().ID != IDlist[j])
               i++;
         
            shapes.add( ((SKBaseShape)pnl.getComponent(i)).getSelectable() );
         }
      
      //Read children
         cnt = p.readInt();
         for (i=0; i<cnt; i++)
         {
            SKGroupTreeNode node = new SKGroupTreeNode(this,-1,"");
            children.add(node);
            node.readFromStream(p,pnl);
         }
      }
   
      public void clearGroupRect()
      {
         groupRect.w = 0;
      }
   
      public void severTies()
      {
         int i;
         for (i=0; i<children.size(); i++)
            children.get(i).severTies();
      
         children.clear();
         shapes.clear();
      }
   
      public int findMaxID(int currMax)
      {
         if (ID > currMax)
            currMax = ID;
      
         int num;
         for (int i=0; i<children.size(); i++)
         {
            num = children.get(i).findMaxID(currMax);
            if (num > currMax)
               currMax = num;
         }
      
         return currMax;
      }
   
      public String toString()
      {
         return name;
      }
   
      public void convertToArray(SKGTNArray array)
      {
         array.add(this);
         System.out.println("111 added group "+name+" shapes size="+shapes.size());
         for(int i=0; i<children.size(); i++)
         {
            children.get(i).convertToArray(array);
         }
      }
   }
