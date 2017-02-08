using Xunit;
using DWriteSharp;
using System.Text;

namespace DWriteSharpTest
{
    public class DirectWriteMeshTextRendererTests
    {
        [Fact]
        public void ThingGetsObjectValFromNumber()
        {
            /* create */
            const string fontFamilyName = "Consolas";
            const FontWeight fontWeight = FontWeight.Normal;
            const FontStyle fontStyle = FontStyle.Normal;
            const FontStretch fontStretch = FontStretch.Normal;
            const float fontSize = 200;
            var textFormat = DWrite.CreateTextFormat(fontFamilyName, fontWeight, fontStyle, fontStretch, fontSize);
            textFormat.TextAlignment = TextAlignment.Leading;

            var text = "DO";// 你好！ こんにちは 안녕하세요
            int maxWidth = 200;
            int maxHeight = 200;

            var textLayout = DWrite.CreateTextLayout(text, textFormat, maxWidth, maxHeight);

            DWrite.RenderLayoutToMesh(textLayout, 0, 0, AddLine, AddBezier, ClosePath, FigureBegin, FigureEnd);

            SaveToTextFile("D:\\out.txt");
        }

        static StringBuilder builder = new StringBuilder(1024);

        public static void AddLine(float x, float y)
        {
            builder.AppendLine(string.Format("line to ({0:0.00}, {1:0.00})", x, y));
        }

        public static void AddBezier(float x0, float y0, float x1, float y1, float x2, float y2)
        {
            builder.AppendLine(string.Format("bezier a ({0:0.00}, {1:0.00}) b ({2:0.00}, {3:0.00}) c ({4:0.00}, {5:0.00})", x0, y0, x1, y1, x2, y2));
        }

        public static void ClosePath()
        {
            builder.AppendLine("Path closed.");
        }

        public static void FigureBegin(float x, float y)
        {
            builder.AppendLine(string.Format("Figure begin at({0:0.00}, {1:0.00}).", x, y));
        }

        public static void FigureEnd()
        {
            builder.AppendLine("Figure end.");
        }


        public static void SaveToTextFile(string filePath)
        {
            if (System.IO.File.Exists(filePath))
            {
                System.IO.File.Delete(filePath);
            }
            System.IO.File.WriteAllText(filePath, builder.ToString());
            builder.Clear();
        }

        public static void SaveToObjFile(string filePath, float[] positions, int[] indexes)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append("# MTE");
            sb.AppendLine();

            for (int i = 0; i < positions.Length; i+=2)
            {
                sb.AppendFormat("v {0} {1} 0", positions[i], positions[i+1]);
                sb.AppendLine();
            }

            sb.AppendLine("v -100 -100 0");
            sb.AppendLine("v -100 100 0");
            sb.AppendLine("v 100 100 0");
            sb.AppendLine("v 100 -100 0");

            sb.AppendLine("v -10 0 0");
            sb.AppendLine("v 0 0 100");
            sb.AppendLine("v 10 0 0");

            for (int i = 0; i < positions.Length; i++)
            {
                sb.AppendFormat("vt {0} {1}", 0, 0);
                sb.AppendLine();
            }

            sb.AppendLine("vt 0 0");
            sb.AppendLine("vt 0 0");
            sb.AppendLine("vt 0 0");
            sb.AppendLine("vt 0 0");

            sb.AppendLine("vt 0 0");
            sb.AppendLine("vt 0 0");
            sb.AppendLine("vt 0 0");

            for (int i = 0; i < indexes.Length; i += 3)
            {
                sb.AppendFormat("f {0}/{0} {1}/{1} {2}/{2}",
                    indexes[i] + 1,
                    indexes[i + 1] + 1,
                    indexes[i + 2] + 1);
                sb.AppendLine();
            }

            sb.AppendFormat("f {0}/{0} {1}/{1} {2}/{2}", positions.Length/2 + 1, positions.Length/2 + 2, positions.Length/2 + 3);
            sb.AppendLine();
            sb.AppendFormat("f {0}/{0} {1}/{1} {2}/{2}", positions.Length/2 + 3, positions.Length/2 + 4, positions.Length/2 + 1);
            sb.AppendLine();

            sb.AppendFormat("f {0}/{0} {1}/{1} {2}/{2}", positions.Length/2 + 5, positions.Length/2 + 6, positions.Length/2 + 7);
            sb.AppendLine();

            System.IO.File.WriteAllText(filePath, sb.ToString());
        }
    }
}
