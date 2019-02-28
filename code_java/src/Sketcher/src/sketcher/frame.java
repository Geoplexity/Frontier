   import javax.swing.JScrollPane;
   import javax.swing.JFrame;

   public class frame extends JFrame
   {
      public static JScrollPane pane = new JScrollPane();
      public static void main(String[] args)
      {
      frame f = new frame();
         f.add(pane);
         f.show();
      }
      public frame()
      {
      super();
      }
   }