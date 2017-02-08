using Xunit;
using DWriteSharp;
using System.Text;

namespace DWriteSharpTest
{
    public class DWriteMeshTextRendererTest
    {
        const bool WriteMeshToText = true;
        const string MeshTextPath = "D:\\mesh_text.txt";
        StringBuilder builder;

        const bool DrawPath = true;
        const string PathImagePath = "D:\\path_image.png";
        Cairo.ImageSurface surface;
        Cairo.Context g;

        #region renderer handlers

        public void AddLine(float x, float y)
        {
            if (WriteMeshToText)
            {
                builder.AppendLine(string.Format("line to ({0:0.00}, {1:0.00})", x, y));
            }

            if (DrawPath)
            {
                g.LineTo(x, y);
            }
        }

        public void AddBezier(float c0x, float c0y, float c1x, float c1y, float p1x, float p1y)
        {
            if (WriteMeshToText)
            {
                builder.AppendLine(string.Format("bezier c0 ({0:0.00}, {1:0.00}) c1 ({2:0.00}, {3:0.00}) end ({4:0.00}, {5:0.00})", c0x, c0y, c1x, c1y, p1x, p1y));
            }

            if (DrawPath)
            {
                var p = new { X = (c0x + c1x) / 2, Y = (c0y + c1y) / 2 };
                g.LineTo(p.X, p.Y);
                g.LineTo(p1x, p1y);
            }
        }

        public void ClosePath()
        {
            if (WriteMeshToText)
            {
                builder.AppendLine("Path closed.");
            }
        }

        public void FigureBegin(float x, float y)
        {
            if (WriteMeshToText)
            {
                builder.AppendLine(string.Format("Figure begin at({0:0.00}, {1:0.00}).", x, y));
            }

            if (DrawPath)
            {
                g.MoveTo(x, y);
            }
        }

        public void FigureEnd()
        {
            if (WriteMeshToText)
            {
                builder.AppendLine("Figure end.");
            }
            if (DrawPath)
            {
                g.ClosePath();
            }
        }

        #endregion

        [Fact]
        public void ShouldGetCorrectOutline()
        {
            // create a TextFormat
            const string fontFamilyName = "Consolas";
            const FontWeight fontWeight = FontWeight.Normal;
            const FontStyle fontStyle = FontStyle.Normal;
            const FontStretch fontStretch = FontStretch.Normal;
            const float fontSize = 200;
            var textFormat = DWrite.CreateTextFormat(fontFamilyName, fontWeight, fontStyle, fontStretch, fontSize);
            textFormat.TextAlignment = TextAlignment.Leading;

            // create a TextLayout
            var text = "WO";
            int maxWidth = 200;
            int maxHeight = 200;
            var textLayout = DWrite.CreateTextLayout(text, textFormat, maxWidth, maxHeight);
            
            // prepare debug contexts
            if(WriteMeshToText)
            {
                builder = new StringBuilder(1024);
            }
            if (DrawPath)
            {
                surface = new Cairo.ImageSurface(Cairo.Format.Argb32, 800, 600);
                g = new Cairo.Context(surface);
            }

            // render the textlayout
            DWrite.RenderLayoutToMesh(textLayout, 0, 0, AddLine, AddBezier, ClosePath, FigureBegin, FigureEnd);

            // show debug results
            if (WriteMeshToText)
            {
                if (System.IO.File.Exists(MeshTextPath))
                {
                    System.IO.File.Delete(MeshTextPath);
                }
                System.IO.File.WriteAllText(MeshTextPath, builder.ToString());
            }
            if (DrawPath)
            {
                g.Stroke();
                surface.WriteToPng(PathImagePath);
                g.Dispose();
                surface.Dispose();

                Assert.False(PathImagePath.Contains(" "));
                // open the image in Windows Photo Viewer
                System.Diagnostics.Process.Start(@"C:\WINDOWS\System32\rundll32.exe", @"""C:\Program Files\Windows Photo Viewer\PhotoViewer.dll"", ImageView_Fullscreen " + PathImagePath);
            }
            
            // manually check if the output text and image shows a right outline of the text

            // clean up
            textLayout.Dispose();
            textFormat.Dispose();
        }

    }
}
