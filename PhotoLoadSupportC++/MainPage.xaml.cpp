//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace PhotoLoadSupportC__;

using namespace Platform;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Search;
using namespace Windows::Storage::BulkAccess;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
}
void LogMessage(Object^ parameter)
{
	auto paraString = parameter->ToString();
	auto formattedText = std::wstring(paraString->Data()).append(L"\r\n");
	OutputDebugString(formattedText.c_str());
}

void PhotoLoadSupportC__::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;
	openPicker->FileTypeFilter->Append(".png");
	create_task(openPicker->PickSingleFileAsync()).then([this](StorageFile^ file)
	{
		if (file)
		{
			//TO DO operate the file 

			//loader other the same type  file
			auto fileTypeFilter = ref new Vector<String^>();
			fileTypeFilter->Append(".png");

			// Create query options.
			auto queryOptions = ref new QueryOptions(CommonFileQuery::OrderByName, fileTypeFilter);
			create_task(KnownFolders::GetFolderForUserAsync(nullptr /* current user */, KnownFolderId::PicturesLibrary)).then([this, queryOptions](StorageFolder^ picturesFolder)
			{
				auto query = picturesFolder->CreateFileQueryWithOptions(queryOptions);
				return query->GetFilesAsync();
			}).then([=](IVectorView<StorageFile^>^ files)
			{
				std::for_each(begin(files), end(files), [=](StorageFile^ file)
				{
					// Create UI elements for the output and fill in the contents when they are available.
					LogMessage(file->Name);
				});
			});
		}
		else
		{
			//fail to get file 
		}
	});
}


void PhotoLoadSupportC__::MainPage::Button_Click1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	FolderPicker^ folderPicker = ref new FolderPicker();
	folderPicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;

	// Users expect to have a filtered view of their folders depending on the scenario.
	// For example, when choosing a documents folder, restrict the filetypes to documents for your application.
	folderPicker->FileTypeFilter->Append("*");

	create_task(folderPicker->PickSingleFolderAsync()).then([this](StorageFolder^ folder)
	{
		if (folder)
		{
			//loader other the same type  file
			auto fileTypeFilter = ref new Vector<String^>();
			fileTypeFilter->Append(".png");

			// Create query options.
			auto queryOptions = ref new QueryOptions(CommonFileQuery::OrderByName, fileTypeFilter);
			auto query = folder->CreateFileQueryWithOptions(queryOptions);
			create_task(query->GetFilesAsync()).then([this](IVectorView<StorageFile^>^ files) {

				std::for_each(begin(files), end(files), [=](StorageFile^ file)
				{
					// Create UI elements for the output and fill in the contents when they are available.
					LogMessage(file->Name);
				});
			});
		}
		else
		{
		}
	});
}
