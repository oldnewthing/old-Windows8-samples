//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

//
// ExtractTextSegments.xaml.cpp
// Implementation of the ExtractTextSegments class
//

#include "pch.h"
#include "ExtractTextSegments.xaml.h"
#include "MainPage.xaml.h"

using namespace Platform;

using namespace SDKSample;
using namespace SDKSample::TextSegmentation;

using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;


ExtractTextSegments::ExtractTextSegments()
{
    InitializeComponent();
}

/// <summary>
/// This is the click handler for the 'Word Segments' button.
///
/// When this button is activated, the Text Segmentation API will calculate
/// the word segments from the given input string and language tag, and then
/// print out the word and alternate-form segments for that language.
///
/// </summary>
/// <param name="sender">The object that raised the event.</param>
/// <param name="e">Event data that describes the click action on the button.</param>
void TextSegmentation::ExtractTextSegments::WordSegmentsButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Button^ b = dynamic_cast<Button^>(sender);
    if (b != nullptr)
    {
        // Initialize and obtain input values
        String^ notifyText = ref new String();

        // Obtain the input string value, check for non-emptiness
        String^ inputStringText = inputStringBox->Text;
        if (inputStringText->IsEmpty())
        {
            notifyText = "\nCannot compute word segments: input string is empty.";
            MainPage::Current->NotifyUser(notifyText, NotifyType::ErrorMessage);
            return;
        }

        // Obtain the language tag value, check for non-emptiness and that it is valid
        // Ex. Valid Values:
        //     "en-US" (English (United States))
        //     "fr-FR" (French (France))
        //     "de-DE" (German (Germany))
        //     "ja-JP" (Japanese (Japan))
        //     "ar-SA" (Arabic (Saudi Arabia))
        //     "zh-CN" (China (PRC))
        String^ languageTagText = languageTagBox->Text;
        if (languageTagText->IsEmpty())
        {
            notifyText += "\nLanguage tag input is empty ... using generic-language segmentation rules.";
            languageTagText = "und";    // This is used for non language-specific locales. 'und' is short for 'undetermined'.
        }
        else
        {
            if (!Windows::Globalization::Language::IsWellFormed(languageTagText))
            {
                notifyText = "\nLanguage tag is not well formed: \"" + languageTagText + "\"";
                MainPage::Current->NotifyUser(notifyText, NotifyType::ErrorMessage);
                return;
            }
        }

        // Notify that we are going to calculate word segments
        notifyText += "\n\nCalculating word segments ...\n";
        notifyText += "\nInput: \"" + inputStringText + "\"";
        notifyText += "\nLanguage Tag: \"" + languageTagText + "\"";

        // Call the TextSegmentation API to calculate the word boundaries, add them to a StringBuilder instance
        auto segmenter = ref new Windows::Data::Text::WordsSegmenter(languageTagText);
        auto wordSegments = segmenter->GetTokens(inputStringText);

        // Iterate over the word segments, add words and alternates to notifyText
        for (unsigned int iWordSegment = 0; iWordSegment != wordSegments->Size; iWordSegment++)
        {
            auto wordSegment = wordSegments->GetAt(iWordSegment);
            notifyText += "\n\tWord---->\"" + wordSegment->Text + "\"";

            auto alternateForms = wordSegment->AlternateForms;
            for (unsigned int iAlternateForm = 0; iAlternateForm != alternateForms->Size; iAlternateForm++)
            {
                auto alternateForm = alternateForms->GetAt(iAlternateForm);
                notifyText += "\n\t\tAlternate---->\"" + alternateForm->AlternateText + "\"";
            }
        }

        // send notifyText to the output pane
        MainPage::Current->NotifyUser(notifyText, NotifyType::StatusMessage);
    }
}

/// <summary>
/// This is the click handler for the 'Selection Segments' button.
///
/// When this button is activated, the Text Segmentation API will calculate
/// the selection segments from the given input string and language tag, and then
/// print out the word and alternate-form segments for that language.
///
/// Selection segments differ from word segments in that they describe the bounds
/// between active-selection snapping behavior.
///
/// </summary>
/// <param name="sender">The object that raised the event.</param>
/// <param name="e">Event data that describes the click action on the button.</param>
void TextSegmentation::ExtractTextSegments::SelectionSegmentsButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Button^ b = dynamic_cast<Button^>(sender);
    if (b != nullptr)
    {
        // Initialize and obtain input values
        String^ notifyText = ref new String();

        // Obtain the input string value, check for non-emptiness
        String^ inputStringText = inputStringBox->Text;
        if (inputStringText->IsEmpty())
        {
            notifyText = "\nCannot compute selection segments: input string is empty.";
            MainPage::Current->NotifyUser(notifyText, NotifyType::ErrorMessage);
            return;
        }

        // Obtain the language tag value, check for non-emptiness and that it is valid
        // Ex. Valid Values:
        //     "en-US" (English (United States))
        //     "fr-FR" (French (France))
        //     "de-DE" (German (Germany))
        //     "ja-JP" (Japanese (Japan))
        //     "ar-SA" (Arabic (Saudi Arabia))
        //     "zh-CN" (China (PRC))
        String^ languageTagText = languageTagBox->Text;
        if (languageTagText->IsEmpty())
        {
            notifyText += "\nLanguage tag input is empty ... using generic-language segmentation rules.";
            languageTagText = "und";    // This is used for non language-specific locales. 'und' is short for 'undetermined'.
        }
        else
        {
            if (!Windows::Globalization::Language::IsWellFormed(languageTagText))
            {
                notifyText = "\nLanguage tag is not well formed: \"" + languageTagText + "\"";
                MainPage::Current->NotifyUser(notifyText, NotifyType::ErrorMessage);
                return;
            }
        }

        // Notify that we are going to calculate selection segments
        notifyText += "\n\nCalculating selection segments ...\n";
        notifyText += "\nInput: \"" + inputStringText + "\"";
        notifyText += "\nLanguage Tag: \"" + languageTagText + "\"";

        // Call the TextSegmentation API to calculate the word boundaries, add them to a StringBuilder instance
        auto segmenter = ref new Windows::Data::Text::SelectableWordsSegmenter(languageTagText);
        auto selectionSegments = segmenter->GetTokens(inputStringText);

        // Iterate over the selection segments, add segments to notifyText
        for (unsigned int iSelectionSegment = 0; iSelectionSegment != selectionSegments->Size; iSelectionSegment++)
        {
            auto selectionSegment = selectionSegments->GetAt(iSelectionSegment);
            notifyText += "\n\tWord---->\"" + selectionSegment->Text + "\"";
        }

        // send notifyText to the output pane
        MainPage::Current->NotifyUser(notifyText, NotifyType::StatusMessage);
    }
}
