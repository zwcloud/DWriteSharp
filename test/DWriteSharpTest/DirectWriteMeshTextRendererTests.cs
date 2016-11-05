using NUnit.Framework;
using DWriteSharp;
using System.Text;

namespace DWriteSharpTest
{
    [TestFixture]
    public class DirectWriteMeshTextRendererTests
    {
        [TestCase]
        public void ThingGetsObjectValFromNumber()
        {
            /* create */
            const string fontFamilyName = "SimSun";
            const FontWeight fontWeight = FontWeight.Bold;
            const FontStyle fontStyle = FontStyle.Normal;
            const FontStretch fontStretch = FontStretch.Normal;
            const float fontSize = 12f;
            var textFormat = DWrite.CreateTextFormat(fontFamilyName, fontWeight, fontStyle, fontStretch, fontSize);
            textFormat.TextAlignment = TextAlignment.Leading;

            var text = "你好！ こんにちは 안녕하세요";
            int maxWidth = 2000;
            int maxHeight = 1000;

            var textLayout = DWrite.CreateTextLayout(text, textFormat, maxWidth, maxHeight);

            int[] indexArray;
            float[] vertexArray;
            DWrite.RenderLayoutToMesh(textLayout, 0, 0, out indexArray, out vertexArray);

            if (indexArray.Length != 0)
            {
                var objFilePath = "D:\\dwrite_output_mesh_test.obj";
                SaveToObjFile(objFilePath, vertexArray, indexArray);

                System.Diagnostics.Process.Start(@"E:\Program Files\Autodesk\FBX Review\fbxreview.exe", objFilePath);
            }
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
