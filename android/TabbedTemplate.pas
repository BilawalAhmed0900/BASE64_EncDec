unit TabbedTemplate;

interface

uses
  System.SysUtils, System.Types, System.UITypes, System.Classes, System.Variants,
  FMX.Types, FMX.Graphics, FMX.Controls, FMX.Forms, FMX.Dialogs, FMX.TabControl,
  FMX.StdCtrls, FMX.Gestures, FMX.ScrollBox, FMX.Memo, FMX.Controls.Presentation,
  System.NetEncoding, EncdDecd, System.IoUtils;

type
  TTabbedForm = class(TForm)
    HeaderToolBar: TToolBar;
    ToolBarLabel: TLabel;
    TabControl1: TTabControl;
    TabItem1: TTabItem;
    TabItem2: TTabItem;
    GestureManager1: TGestureManager;
    ToEncodeMemo: TMemo;
    ToEncodeButton: TButton;
    EncodeUndoButton: TButton;
    ToDecodeMemo: TMemo;
    ToDecodeButton: TButton;
    DecodeUndoButton: TButton;
    procedure FormCreate(Sender: TObject);
    procedure FormGesture(Sender: TObject; const EventInfo: TGestureEventInfo; var Handled: Boolean);
    procedure ToEncodeButtonClick(Sender: TObject);
    procedure EncodeUndoButtonClick(Sender: TObject);
    procedure ToDecodeButtonClick(Sender: TObject);
    procedure DecodeUndoButtonClick(Sender: TObject);
  private
    ToEncodeString: string;
    ToDecodeString: string;
    { Private declarations }
  public
    { Public declarations }
  end;

var
  TabbedForm: TTabbedForm;

implementation

{$R *.fmx}

procedure TTabbedForm.FormCreate(Sender: TObject);
begin
  { This defines the default active tab at runtime }
  TabControl1.ActiveTab := TabItem1;
  ToEncodeString        := '';
end;

procedure TTabbedForm.FormGesture(Sender: TObject; const EventInfo: TGestureEventInfo; var Handled: Boolean);
begin
{$IFDEF ANDROID}
  case EventInfo.GestureID of
    sgiLeft:
      begin
        if TabControl1.ActiveTab <> TabControl1.Tabs[TabControl1.TabCount - 1] then
          TabControl1.ActiveTab := TabControl1.Tabs[TabControl1.TabIndex + 1];
        Handled                 := True;
      end;

    sgiRight:
      begin
        if TabControl1.ActiveTab <> TabControl1.Tabs[0] then
          TabControl1.ActiveTab := TabControl1.Tabs[TabControl1.TabIndex - 1];
        Handled                 := True;
      end;
  end;
{$ENDIF}
end;

procedure TTabbedForm.ToEncodeButtonClick(Sender: TObject);
begin
  ToEncodeString := ToEncodeMemo.Text;
  try
    ToEncodeMemo.Text := EncdDecd.EncodeString(ToEncodeMemo.Text);
  finally
    EncodeUndoButton.Visible := True;
  end;
end;

procedure TTabbedForm.EncodeUndoButtonClick(Sender: TObject);
begin
  ToEncodeMemo.Text        := ToEncodeString;
  ToEncodeString           := '';
  EncodeUndoButton.Visible := False;
end;

procedure TTabbedForm.ToDecodeButtonClick(Sender: TObject);
begin
  ToDecodeString := ToDecodeMemo.Text;
  try
    ToDecodeMemo.Text := EncdDecd.DecodeString(ToDecodeMemo.Text);
  finally
    DecodeUndoButton.Visible := True;
  end;
end;

procedure TTabbedForm.DecodeUndoButtonClick(Sender: TObject);
begin
  ToDecodeMemo.Text        := ToDecodeString;
  ToDecodeString           := '';
  DecodeUndoButton.Visible := False;
end;

end.
