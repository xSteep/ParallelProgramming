using System;
using System.IO;
using System.Text;
using System.Threading.Tasks;

namespace lw7._1
{
    class Program
    {
        static async Task Main()
        {
            Console.Write("Text path: ");
            string filePath = Console.ReadLine();

            Console.Write("Delete char: ");
            string charactersToRemove = Console.ReadLine();

            try
            {
                await RemoveCharactersAsync(filePath, charactersToRemove);
                Console.WriteLine($"Delete {charactersToRemove} succeful.");
            }
            catch (FileNotFoundException)
            {
                Console.WriteLine("Text file not found.");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error {ex.Message}");
            }
        }

        static async Task RemoveCharactersAsync(string filePath, string charactersToRemove)
        {
            string text;
            using (StreamReader reader = new StreamReader(filePath, Encoding.UTF8))
            {
                text = await reader.ReadToEndAsync();
            }

            string cleanedText = RemoveCharacters(text, charactersToRemove);

            using (StreamWriter writer = new StreamWriter(filePath, false, Encoding.UTF8))
            {
                await writer.WriteAsync(cleanedText);
            }
        }

        static string RemoveCharacters(string text, string charactersToRemove)
        {
            StringBuilder cleanedText = new StringBuilder(text.Length);
            foreach (char c in text)
            {
                if (charactersToRemove.IndexOf(c) == -1)
                {
                    cleanedText.Append(c);
                }
            }
            return cleanedText.ToString();
        }
    }
}