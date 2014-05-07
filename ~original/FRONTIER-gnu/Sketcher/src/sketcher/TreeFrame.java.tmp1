/**
 * Title:        SKBifurFrame
 * Description:  Frame used to display bifurcations
 */
   package sketcher;

   import javax.swing.*;
   import java.awt.*;
   import java.awt.event.*;
   import java.awt.Graphics;

   public class TreeFrame extends JDialog
   {
      public static SKMainFrame frame1;
      public static SKGTNArray Groups = new SKGTNArray(100);
      public static int ID,w,h;
      public static JScrollPane pnlShapes = null;
      public static SKScrollableTree panel= null;
      public static int[] flag;
      public static SKText mouseOverItem = null;
      public JPanel pnlBifurButtons = new JPanel();
      public JPanel pnlMain = new JPanel();
      public JPanel pnlButtons = new JPanel();
   
      public BorderLayout borderLayout1 = new BorderLayout();
      public BorderLayout borderLayout2 = new BorderLayout();
   
   
      public TreeFrame(Frame owner, SKGTNArray allGroups, int actID, int[] fin)
      {
         super(owner);
         frame1 = (SKMainFrame)owner;
         allGroups.copyArrayTo(Groups);
         ID = actID;
	 panel = new SKScrollableTree(this,frame1,Groups,null,actID,fin);
         flag = new int[fin.length];
         for(int i=0; i<fin.length; i++)
            flag[i] = fin[i];
         try
         {
            jbInit();
	    panel.drawTree();
            setSize(500,350);
         //Center on desktop
            Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
            Dimension d = getSize();
            int x = (screen.width - d.width) >> 1;
            int y = (screen.height - d.height) >> 1;
           // setLocation(x, y);
         }
            catch(Exception e)
            {
               e.printStackTrace();
            }
      
      }
   
   
      private void jbInit() throws Exception
      {
         pnlMain.setLayout(borderLayout1);
         pnlMain.setBorder(BorderFactory.createLoweredBevelBorder());
      
         this.addWindowListener(
                                 new java.awt.event.WindowAdapter()
                                 {
                                 
                                    public void windowOpened(WindowEvent e)
                                    {
                                       this_windowOpened(e);
                                    }
                                 });
/*	 panel.addMouseListener(
                                 new java.awt.event.MouseAdapter()
                                 {

                                    public void mouseClicked(MouseEvent e)
                                    {
					System.out.println("111 clicked on panel");
                                    }

                                    public void mouseReleased(MouseEvent e)
                                    {
                                    }
                                 });
*/
         this.getContentPane().add(pnlMain, BorderLayout.CENTER);
         pnlMain.add(panel, BorderLayout.CENTER);
      }
	
      public void reDrawTree()
      {
	if(mouseOverItem != null)
	{
	  SKText name = mouseOverItem;
	  String groupName = name.getString();
	  int groupID = -1;
	  switch(groupName.charAt(0))
	  {
	    case 'G': if(groupName.charAt(1)=='r')	break;
	    	      else groupID = Integer.parseInt(groupName.substring(1));
		      break;
	    case 'C': groupID = Integer.parseInt(groupName.substring(1));
		      break;
	    default: break;
	  }
	  if(groupID!=-1)
	  {
	    SKGTNArray childArray = new SKGTNArray(2);
	    SKGroupTreeNode node = Groups.findByID(groupID);	
	    SKGroupTreeNode root = new SKGroupTreeNode(null, node.ID, node.name);
	    node.shapes.copyArrayTo(root.shapes);

	    for(int j=0; j<node.children.size(); j++)
            {
                SKGroupTreeNode child = new SKGroupTreeNode(root,node.children.get(j).ID,node.children.get(j).name);
                node.children.get(j).shapes.copyArrayTo(child.shapes);
                root.children.add(child);
            }

	    if(root!=null)
	    {
	      childArray.add(root);
	      for(int i=0; i<root.children.size(); i++)
		childArray.add(root.children.get(i));
	      int[] newFlag = new int[childArray.size()+1]; 
              newFlag[0] = flag[Groups.indexOf(node)];
              for(int i = 0; i<childArray.size(); i++)
              {
                SKGroupTreeNode g = Groups.findByID(childArray.get(i).ID);
                newFlag[i+1] = flag[Groups.indexOf(g)];
              }

	      panel.Tree=childArray;
	      panel.Root=root;
	      panel.ID=root.ID;
	      panel.flag=newFlag;
	      panel.drawTree();
	    }
	  }
	}
	mouseOverItem = null;
      }

      public void drawOriginalTree()
      {
	  panel.Tree=Groups;
	  panel.Root=null;
	  panel.ID=ID;
	  panel.flag=flag;
	  panel.drawTree();
      }   

      public void this_windowOpened(WindowEvent e)
      {
	this.show();
      }
	
   }
