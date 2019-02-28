/**
 * Title:        SKGroups
 * Description:  Group-related methods
 */
   package sketcher;
   public class SKGroups
   {
      public static void getAllGroupShapes(SKGroupTreeNode node, SKShapeArray shapes, boolean uniqueOnly)
      {    int i;
         for (i=0; i<node.shapes.size(); i++)
            if (!uniqueOnly || (shapes.indexOf(node.shapes.get(i)) == -1))
               shapes.add(node.shapes.get(i));
      
         for (i=0; i<node.children.size(); i++)
            getAllGroupShapes(node.children.get(i),shapes,uniqueOnly);
      }
   
      private static int rGetShapeLevel(SKBaseShape shape, SKGroupTreeNode node, int thisLevel)
      {
         int tmp,
         res = -1;
         if (node.shapes.indexOf(shape) > -1)
            res = thisLevel;
         else
            for (int i=0; i<node.children.size(); i++)
            {
               tmp = rGetShapeLevel(shape,node.children.get(i),thisLevel+1);
               if (tmp > -1)  res = tmp;
            }
      
         return res;
      }
      public static void setLevels(SKGTNArray tree)
      {
         SKGroupTreeNode root = null;
         for(int i = 0; i< tree.size(); i++)
         {
            if(tree.get(i).parents.size() == 0)
            {
               root = tree.get(i);
               markLevels(root,0);
            }
         }
      }   
      public static void markLevels(SKGroupTreeNode root, int l)
      {
      	if(root.level <= l)
         root.level = l;
         for(int i=0; i<root.children.size(); i++)
            markLevels(root.children.get(i),l+1);
      }
      public static void separateGroups(SKGroupTreeNode node, SKGTNArray groups)
      {
         for(int i=0; i< node.children.size(); i++)
            groups.add(node.children.get(i));
      
         for(int i=0; i<node.children.size(); i++)
            separateGroups(node.children.get(i),groups);
      
         return;
      }
   
      public static SKGroupTreeNode getHighestGroupWithShape(SKBaseShape shape, SKGTNArray groups)
      {
         SKGroupTreeNode res = null;
         int tmp, lowestLevel = 999;
         for (int i=0; i<groups.size(); i++)
         {
            tmp = rGetShapeLevel(shape,groups.get(i),0);
            if (tmp>-1 && tmp<lowestLevel)
            {
               lowestLevel = tmp;
               res = groups.get(i);
            }
         }
      
         return res;
      }
   
   //Returns TRUE iff this group contains only items in shapes
      private static boolean removeShapesInGroup(SKGroupTreeNode node, SKShapeArray shapes)
      {
         boolean res=true;
         int i;
         for (i=0; i<node.shapes.size(); i++)
            res = res && shapes.removeShape(node.shapes.get(i));
      
         for (i=0; i<node.children.size(); i++)
            res = res && removeShapesInGroup(node.children.get(i),shapes);
      
         return res;
      }
   
      public static SKGroupTreeNode makeGroup(SKGroupTreeNode root, SKShapeArray shapes, int groupID, int treeID)
      {
      //Find group with as many of these shapes as possible
         SKShapeArray foundShapes = new SKShapeArray(shapes.size()),
         otherShapes = new SKShapeArray(shapes.size()),
         tempShapes  = new SKShapeArray(shapes.size());
         SKGTNArray   foundNodes = new SKGTNArray(2),
         tmpGroups   = new SKGTNArray(3);
         boolean      isSubsetGroup;
         SKGroupTreeNode node = root.children.get(treeID);
         shapes.copyArrayTo(otherShapes);
      
         SKGroupTreeNode thisNode,foundNode = null;
         int i,g,j;
      
      //Is this a subset of any existing groups?
         for (i=0; i<node.children.size(); i++)
         {
            thisNode = node.children.get(i);
            foundShapes.clear();
            getAllGroupShapes(thisNode,foundShapes,true);
            isSubsetGroup = true;
            for (j=0; isSubsetGroup && j<shapes.size(); j++)
               isSubsetGroup = isSubsetGroup && foundShapes.removeShape(shapes.get(j));
         
            if (isSubsetGroup)
            {
               SKGroupTreeNode res = new SKGroupTreeNode(thisNode,groupID,"Group"+groupID);
               shapes.copyArrayTo(res.shapes);
               //System.out.println("parent:"+thisNode);
            
               thisNode.children.add(res);
            
               return res;
            }
         }
      
      //Are any groups a subset of this group?
         for (i=0; i<node.children.size(); i++)
         {
            thisNode = node.children.get(i);
            shapes.copyArrayTo(foundShapes);
            isSubsetGroup = removeShapesInGroup(thisNode,foundShapes);
            if (isSubsetGroup && foundShapes.size()<shapes.size())
            {
               foundNodes.add(thisNode);
            }
         
         //Check to see if combining 2 or more groups would form this group
            if (isSubsetGroup)
            {
            //If foundShapes is empty, then this would be a redundant group
               if (foundShapes.size()==0) 
                  return thisNode;
            
               tmpGroups.clear();
               tmpGroups.add(thisNode);
               foundShapes.copyArrayTo(tempShapes);
               for (g=i+1; g<node.children.size(); g++)
               {
                  removeShapesInGroup(node.children.get(g),foundShapes);
                  if (tempShapes.size()-foundShapes.size() > 0) // == node.children.get(g).shapes.size())
                  {
                     tmpGroups.add(node.children.get(g));
                  
                  //Does this completely contain all shapes?
                     if (foundShapes.size()==0)
                     {
                        for (j=0; j<tmpGroups.size(); j++)
                           node.children.removeNode(tmpGroups.get(j));
                     
                        SKGroupTreeNode res = new SKGroupTreeNode(node,groupID,"Group"+groupID);
                        tmpGroups.copyArrayTo(res.children);
                        node.children.add(res);
                        return res;
                     }
                  
                     foundShapes.copyArrayTo(tempShapes);
                  }
                  else tempShapes.copyArrayTo(foundShapes);
               }
            }
         }
      
         if (foundNodes.size() == 0)
         { //No groups contained ANY of the shapes
            SKGroupTreeNode res = new SKGroupTreeNode(node,groupID,"Group"+groupID);
            shapes.copyArrayTo(res.shapes);
            node.children.add(res);
            return res;
         }
         else
         {
            if (otherShapes.size()>0)
            { //foundNode contains some (but not all) of shapes
               SKGroupTreeNode res = new SKGroupTreeNode(node,groupID,"Group"+groupID);
               node.children.add(res);
            
            //Move subgroups into res's children
               otherShapes.clear();
               for (i=0; i<foundNodes.size(); i++)
               {
                  foundNode = foundNodes.get(i);
                  getAllGroupShapes(foundNode,otherShapes,true);
               
                  foundNode.parent.children.removeNode(foundNode);
                  foundNode.parent = res;
                  res.children.add(foundNode);
               }
            
               shapes.copyArrayTo(res.shapes);
               for (i=0; i<otherShapes.size(); i++)
                  res.shapes.removeShape( otherShapes.get(i) );
            
               return res;
            }
            else
            { //foundNode contains ALL shapes plus some, recurse deeper
               for (i=0; i<foundNodes.size(); i++)
                  return makeGroup(foundNodes.get(i),shapes,groupID,treeID);
            }
         }
      
         return null;
      }
   
      public static SKGroupTreeNode makeGrouptree(SKGroupTreeNode root, SKShapeArray shapes, int groupID, SKGTNArray allGroups, int treeID, int fix)
      
      {
      
         SKShapeArray tempShapes1  = new SKShapeArray(shapes.size());
         SKShapeArray shapes1=new SKShapeArray(100);
         SKGTNArray   Bigger = new SKGTNArray(100),
         Smaller  = new SKGTNArray(100);
         SKGroupTreeNode temp=null;
         int ID=groupID;
      
         SKGroupTreeNode node = root.children.get(treeID);
         SKGroupTreeNode Node=new SKGroupTreeNode(null,ID,"U"+(treeID+1)+"Group"+ID);
         SKGTNArray Nchildren=new SKGTNArray(100),
         Nparents= new SKGTNArray(100);
         shapes.copyArrayTo(Node.shapes);
         shapes.copyArrayTo(shapes1);
         Node.fixed = fix;
         for(int x=0;x<shapes.size(); x++)
            if(node.shapes.indexOf(shapes.get(x))==-1)
               node.shapes.add(shapes.get(x));
      
         if(groupID==1)
         { node.children.add(Node);
            Node.parents.add(node);
         }
         else
         {
            for(int a=0; a<allGroups.size(); a++)
            {
               if (allGroups.get(a).shapes.size()>shapes.size())
                  Bigger.add(allGroups.get(a));
               else if (allGroups.get(a).shapes.size()<shapes.size())
                  Smaller.add(allGroups.get(a));
            }
         
         
            SKGTNArray copyb= new SKGTNArray(100);
            Bigger.copyArrayTo(copyb);
            for(int i=0;i<copyb.size();i++)
            {//System.out.println("size:"+copyb.size());
               copyb.get(i).shapes.copyArrayTo(tempShapes1);
               boolean yes1=true;
               for (int x=0; x<Node.shapes.size(); x++)
                  yes1 = yes1 && tempShapes1.removeShape(Node.shapes.get(x));
               if(yes1)
               {//System.out.println("GROUP"+copyb.get(i));
                  temp=copyb.get(i);
                  for(int c=0; c<temp.parents.size(); c++)
                     for(int d=0; d< Bigger.size();d++)
                        if(Bigger.get(d)==temp.parents.get(c))
                        {//System.out.println("removed node: "+Bigger.get(d));
                           Bigger.removeNode(Bigger.get(d));}
               }
               else  
               {//System.out.println("temp:"+copyb.get(i));
                  for(int d=0; d< Bigger.size();d++)
                  {//System.out.println("big:"+Bigger.get(d));
                     if(Bigger.get(d)==copyb.get(i))
                     {//System.out.println("removed node: "+Bigger.get(d));
                        Bigger.removeNode(Bigger.get(d));}
                  }
               }
            
            }
         
         
            Bigger.copyArrayTo(Node.parents);
         
            SKGTNArray copys= new SKGTNArray(100);
            Smaller.copyArrayTo(copys);
            tempShapes1.clear();
            for(int i=0;i<copys.size();i++)
            {shapes.copyArrayTo(tempShapes1);
               boolean yes=true;
               for (int x=0; x<copys.get(i).shapes.size(); x++)
                  yes = yes && tempShapes1.removeShape(copys.get(i).shapes.get(x));
               if(yes)
               {//Sysem.out.println("GROUP"+copys.get(i));
                  temp=copys.get(i);
                  for(int c=0; c<temp.children.size(); c++)
                     for(int d=0; d< Smaller.size();d++)
                        if(Smaller.get(d)==temp.children.get(c))
                        {//System.out.println("removed node: "+Smaller.get(d));
                           Smaller.removeNode(Smaller.get(d));}
               }
               else  
                  for(int d=0; d< Smaller.size();d++)
                     if(Smaller.get(d)==copys.get(i))
                     {//System.out.println("removed node: "+Smaller.get(d));
                        Smaller.removeNode(Smaller.get(d));}
            }
         
         
            Smaller.copyArrayTo(Node.children);
         }
         if(Node.shapes.size()==node.shapes.size())
         {Bigger.clear();
            Bigger.add(node);
            Bigger.copyArrayTo(Node.parents);
            for(int i=0; i<node.children.size(); i++)
            {SKGTNArray copy = new SKGTNArray(100);
               node.children.get(i).parents.copyArrayTo(copy);
               if(copy.indexOf(node)==-1)copy.add(node);
               copy.copyArrayTo(node.children.get(i).parents);
            }
            Smaller.clear();
            Smaller.add(Node);
            Smaller.copyArrayTo(node.children);
         }
      
         return Node;
      }
   
   
      public static boolean shapeInGroup(SKGroupTreeNode node, SKBaseShape shape)
      {
         if (node.shapes.indexOf(shape) != -1)  
            return true;
      
         for (int i=0; i<node.children.size(); i++)
            if (shapeInGroup(node.children.get(i),shape)) 
               return true;
      
         return false;
      }
   
      public static int shapeInGroups(SKBaseShape shape, SKGTNArray groups)
      {
         for (int i=0; i<groups.size(); i++)
            if (shapeInGroup(groups.get(i),shape))  
               return i;
      
         return -1;
      }
   
      public static IBox getRectForGroup(SKGroupTreeNode node)
      {
         int left   = 9999,
         right  = 0,
         top    = 9999,
         bottom = 0;
         SKShapeArray shapes = new SKShapeArray(5);
         getAllGroupShapes(node,shapes,true);
      
         SKBaseShape sh;
         for (int i=0; i<shapes.size(); i++)
         {
            sh = shapes.get(i);
            if (sh.getX() < left)  left = sh.getX();
            if (sh.getY() < top)  top = sh.getY();
            if (sh.getX()+sh.getWidth() > right)  right = sh.getX()+sh.getWidth();
            if (sh.getY()+sh.getHeight() > bottom)  bottom = sh.getY()+sh.getHeight();
         }
      
         return new IBox(left,top,right-left,bottom-top);
      }
   
      public static void removeShapeFromGroup(SKBaseShape shape, SKGroupTreeNode node, SKGTNArray selGroups)
      {
         boolean del = false;
         int i = node.shapes.indexOf(shape);
         if (i > -1)
         {
            node.shapes.remove(i);
            if (node.shapes.size() == 0)
            {
               del = (node.children.size() == 0);
               if (!del)
               {
                  SKShapeArray shapes = new SKShapeArray(5);
                  getAllGroupShapes(node,shapes,true);
                  del = (shapes.size() == 0);
               }
            
               if (del)
               {
                  selGroups.removeNode(node);
                  deleteNode(node);
               }
            }
         }
      
         if (!del)
            for (i=0; i<node.children.size(); i++)
               removeShapeFromGroup(shape,node.children.get(i),selGroups);
      }
   
      public static void deleteNode(SKGroupTreeNode node)
      {
         for (int i=0; i<node.children.size(); i++)
            deleteNode(node.children.get(i));
      
         node.children = null;
         node.shapes = null;
         node.parent.children.removeNode(node);
         node.parent = null;
         node.groupRect = null;
      }
      public static void printTreeWithLevels(SKGroupTreeNode root)
      {
         System.out.println("node = "+root.name+" level "+root.level);
         for(int i=0; i<root.children.size(); i++)
            printTreeWithLevels(root.children.get(i));
      }
   }
