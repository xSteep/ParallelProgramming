using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Net.Http;
using System.Threading.Tasks;

namespace lw7._2
{
    class Program
    {
        static async Task Main()
        {
            const string apiUrl = "https://dog.ceo/api/breeds/image/random";
            const int numberOfImages = 10;

            // Синхронное выполнение
            Console.WriteLine("Sync:");
            var syncStopwatch = Stopwatch.StartNew();
            DownloadImagesSync(apiUrl, numberOfImages);
            syncStopwatch.Stop();
            Console.WriteLine($"Time sync: {syncStopwatch.ElapsedMilliseconds} ms");

            Console.WriteLine("\nAsync:");
            var asyncStopwatch = Stopwatch.StartNew();
            await DownloadImagesAsync(apiUrl, numberOfImages);
            asyncStopwatch.Stop();
            Console.WriteLine($"Time async: {asyncStopwatch.ElapsedMilliseconds} ms");

            Console.ReadLine(); // Для удержания консоли открытой
        }

        static void DownloadImagesSync(string apiUrl, int numberOfImages)
        {
            var httpClient = new HttpClient();

            Console.WriteLine("Download images");

            for (int i = 0; i < numberOfImages; i++)
            {
                Console.WriteLine($"Starting download url: {apiUrl}");
                try
                {
                    var response = httpClient.GetStringAsync(apiUrl).Result;

                    Console.WriteLine($"Image from url: {response} successful");
                }
                catch (HttpRequestException ex)
                {
                    Console.WriteLine($"Error downloading image: {ex.Message}");
                }
            }

            httpClient.Dispose();
        }

        static async Task DownloadImagesAsync(string apiUrl, int numberOfImages)
        {
            var httpClient = new HttpClient();

            Console.WriteLine("Download images");

            // Создаем список задач для параллельного выполнения
            var downloadTasks = new List<Task>();

            for (int i = 0; i < numberOfImages; i++)
            {
                // Запускаем асинхронную задачу и добавляем ее в список
                downloadTasks.Add(DownloadImageAsync(httpClient, apiUrl));
            }

            // Ждем завершения всех асинхронных задач
            await Task.WhenAll(downloadTasks);

            httpClient.Dispose();
        }

        static async Task DownloadImageAsync(HttpClient httpClient, string apiUrl)
        {
            Console.WriteLine($"Starting download url: {apiUrl}");

            try
            {
                var response = await httpClient.GetStringAsync(apiUrl);

                Console.WriteLine($"Image from url: {response} successful");
            }
            catch (HttpRequestException ex)
            {
                Console.WriteLine($"Error downloading image: {ex.Message}");
            }
        }
    }
}